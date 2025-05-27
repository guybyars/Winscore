//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightList.cpp: implementation of the CFlightList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "winscoreview.h"
#include "tasklist.h"
#include "FlightList.h"
#include "MainFrm.h"
#include "progressbar.h"
#include "Global_Prototypes.h"
#include "IGCDirectories.h"
#include <Atlbase.h>

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static void SortNoLogArrays(CStringArray &strArContestNo, CStringArray &strArName, CStringArray &strArStatus, int iSortColumn);
static int iSort=0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlightList::CFlightList()
{

}


CFlightList::~CFlightList()
{
	if( IsEmpty() !=0  )  return;
	Purge();
	return;
}


void CFlightList::AddToList(CFlight * pcAddFlight)
	{
	AddTail( (CFlight*)pcAddFlight );
	}



void CFlightList::Purge()
	{

	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(1)
		{
		CFlight* pcFlight=GetNext(pos);
		if( pcFlight ) delete pcFlight;
		if( pos==NULL ) break;
		}
	RemoveAll();
	}

void CFlightList::Purge(CTime cDate)
	{
	int iDay	=cDate.GetDay();
	int iMonth	=cDate.GetMonth();
	int iYear	=cDate.GetYear();

	if( IsEmpty() !=0  )  return;
	POSITION thispos, pos = GetHeadPosition();
	while(pos!=NULL)
		{
		thispos=pos;
		CFlight* pcFlight=GetNext( pos );
		int iFlightDay	=pcFlight->m_iDay;
		int iFlightMonth=pcFlight->m_iMonth;
		int iFlightYear	=pcFlight->m_iYear;

		if( iDay	==iFlightDay		&&
			iMonth	==iFlightMonth	&&
			iYear	==iFlightYear		)
			{
			delete pcFlight;
			RemoveAt(thispos);
			}
		}
	}


CFlight* CFlightList::GetNext(POSITION& pos)
{
	return (CFlight*)CObList::GetNext(pos);
}


CFlight* CFlightList::GetNext(POSITION& pos, CTime cDate, int nDays )
{

	while(pos!=NULL)
		{
		CFlight* pcFlight=GetNext(pos);
		if( !pcFlight ) return NULL;

		int iInputDay	=pcFlight->m_iDay;
		int iInputMonth	=pcFlight->m_iMonth;
		int iInputYear	=pcFlight->m_iYear;
		for( int i=0; i<nDays; i++)
			{
			CTime cCheckDate=cDate+CTimeSpan(i,0,0,0);
			int iDay	=cCheckDate.GetDay();
			int iMonth	=cCheckDate.GetMonth();
			int iYear	=cCheckDate.GetYear();
			if( iDay	==iInputDay		&&
				iMonth	==iInputMonth	&&
				iYear	==iInputYear		)
				{
				return pcFlight;
				}
			}
		}
	return NULL;
}


#define NUM_COLUMNS 19

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
	{
	_T("ID"), 
	_T("Name"), 
	_T("Status"),
	_T("Distance"),
	_T("Warnings"),
	_T("Roll Time"),
	_T("Start Time"),
	_T("Finish Time"),
	_T("Landing Time"),
	_T("Start Gate ID"),
	_T("Turnpoints Achieved"),
	_T("Landing Location"),
	_T("Airfield Bonus"),
	_T("Scored Landing Point"),
	_T("IGC File Name"), 
	_T("Security")
	};


static int _gnColumnWidth[NUM_COLUMNS] = 
{
	50, //id
	150, //name
	140, //status
	140, //distance
	70, //warnings
	90, //roll
	90, //start
	100, //finish
	100, //landing
	100, //landing
	130, //tp acheived
	170, //landing location
	100, //bonus
	130, //progress
	130, //igc
	130 //security
};

static int _gnColumnFormat[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, //id
	LVCFMT_LEFT, //name
	LVCFMT_LEFT, //status
	LVCFMT_LEFT, //distance
	LVCFMT_LEFT, //warnings
	LVCFMT_LEFT, //roll
	LVCFMT_LEFT, //start
	LVCFMT_LEFT, //finish
	LVCFMT_LEFT, //landing
	LVCFMT_LEFT, //landing
	LVCFMT_LEFT, //tp acheived
	LVCFMT_LEFT, //landing location
	LVCFMT_LEFT, //bonus
	LVCFMT_LEFT, //progress
	LVCFMT_LEFT, //igc
	LVCFMT_LEFT //security
};


#define NUM_PRECONTEST_COLUMNS 10

static _TCHAR *_gszPreContestColumnLabel[NUM_PRECONTEST_COLUMNS] =
	{
	_T("Date"), 
	_T("ID"), 
	_T("Name"), 
	_T("Status"),
	_T("ENL (min/max)"),
	_T("MOP (min/max)"),
	_T("Logger FDR ID"),
	_T("Contestant FDR ID"),
	_T("IGC File Name"), 
	_T("Security")
	};


static int _gnPreContestColumnWidth[NUM_PRECONTEST_COLUMNS] = 
{
	100, //Date
	50, //id
	150, //name
	140, //status
	110, //enl
	110, //mop
	120, //fdr id
	120, //cont fdr id
	430, //igc
	130 //security
};

static int _gnPreContestColumnFormat[NUM_PRECONTEST_COLUMNS] = 
{
	LVCFMT_LEFT, //id
	LVCFMT_LEFT, //name
	LVCFMT_LEFT, //status
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
	LVCFMT_LEFT, //
};


void CFlightList::CreateControlColumns(CListCtrl& ListCtrl, bool bPreContest)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	if( bPreContest )
		{
		for(  int i = 0; i<NUM_PRECONTEST_COLUMNS; i++)
			{
				lvc.fmt =_gnPreContestColumnFormat[i];
				lvc.iSubItem = i;
				lvc.pszText = _gszPreContestColumnLabel[i];
				lvc.cx =  _gnPreContestColumnWidth[i];
				ListCtrl.InsertColumn(i,&lvc);
			}
		}
	else
		{
		for(  int i = 0; i<NUM_COLUMNS; i++)
			{
				lvc.fmt =_gnColumnFormat[i];
				lvc.iSubItem = i;
				lvc.pszText = _gszColumnLabel[i];
				lvc.cx =  _gnColumnWidth[i];
				ListCtrl.InsertColumn(i,&lvc);
			}
		}

}

void CFlightList::LoadFlightList(CListCtrl& ListCtrl, CTime cDate, int nDays, EClass eClass, CContestantList &contestantList, EUnits eUnits, int iSortColumn,  bool bPreContest )
{
	CStringArray cStrArray;

	CFlight* pcFlight;
	ListCtrl.DeleteAllItems( );

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcFlight=GetNext(pos,cDate,nDays);
		if( !pcFlight ) continue;
		if( bPreContest  && !pcFlight->IsContestant()) continue;
		if( !bPreContest && !pcFlight->m_eClass==eClass ) continue;

		pcFlight->m_eUnits=eUnits;
		if( bPreContest )
			{
			pcFlight->AddToPreContestList( ListCtrl, contestantList.GetByContestNo(pcFlight->m_strCID), FALSE );
			}
		else
			pcFlight->AddToList( ListCtrl, FALSE );

		RemoveWinscoreEntry(getNoLogKey(pcFlight->m_strCID, cDate));
		cStrArray.Add(pcFlight->m_strCID);

		}

//  Ok, now add the contestants who don't have a flight log and give them the "No Log Found" status
	CStringArray strArContestNo, strArName, strArStatus;
	pos = contestantList.GetHeadPosition();
	while( pos!=NULL)
		{
		CContestant *pContestant=contestantList.GetNext(pos);
		if( pContestant && pContestant->m_eClass==eClass  && !pContestant->HasWithdrawn() )
			{
			bool bFound=false;
			for( int i=0; i<cStrArray.GetSize(); i++ )
				{
				if( pContestant->m_strContestNo==cStrArray[i] )
					{
					bFound=true;
					break;
					}
				}
			if( !bFound && !bPreContest)
				{
				strArContestNo.Add(pContestant->m_strContestNo);
				strArName.Add(pContestant->m_strLastName+", "+pContestant->m_strFirstName);
				strArStatus.Add(GetWinscoreString(getNoLogKey(pContestant->m_strContestNo, cDate),"No Log Found"));
				}
			else
				RemoveWinscoreEntry(getNoLogKey(pContestant->m_strContestNo, cDate));
			}
		}

	SortNoLogArrays(strArContestNo, strArName, strArStatus, iSortColumn);

	for( int i=0; i<strArContestNo.GetCount(); i++ )
		{
		LV_ITEM lvi;
		lvi.iSubItem = 0;
		lvi.iItem = 0;
		lvi.mask = LVIF_PARAM;
		lvi.lParam = (LPARAM)NULL;
		int iItem=ListCtrl.InsertItem(&lvi);
		ASSERT(iItem>=0);
		ListCtrl.SetItemText(iItem,0, strArContestNo.GetAt(i) );
		ListCtrl.SetItemText(iItem,1, strArName.GetAt(i) );
		ListCtrl.SetItemText(iItem,2, strArStatus.GetAt(i) );
		}
	}

void CFlightList::AnalyzeAll(	void* pFrame,
								HWND hwndParent,
							    CTime			cDate,
								EClass			eClass,
								CTurnpointArray &turnpointArray,
								CContestantList &contestantList,
								CTaskList		&taskList,
								BOOL			bOnlyUnanalyzed,
								EUnits			eUnits)
	{

	CMainFrame* pMainFrame=(CMainFrame*)pFrame;
	CWinscoreView*	pcView=(CWinscoreView*)pMainFrame->GetActiveView( );

	CProgressBar cBar;
	CWaitCursor cWait;
	CStringArray strArray;
	CIGCDirectories cIGCDir;

	cIGCDir.FindIGCFiles( cDate,  1, strArray );

	int nAnalysis=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		if( GetNext(pos,cDate)!=NULL ) nAnalysis++;
		}


    // Set the range and increment of the progress bar. 
    cBar.SetRange(0, nAnalysis); 

	CFlight* pcFlight;
	nAnalysis=0;
	CFileFind cFinder;

	pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcFlight=GetNext(pos,cDate);
		if( !pcFlight ) continue;

		cBar.UpdateProgressBar(nAnalysis++);

		if( bOnlyUnanalyzed && pcFlight->GetStatus()!=eNotAnalyzed ) continue;

		if( pcFlight && !pcFlight->IsLocked() )
			{
			if( eClass==pcFlight->m_eClass )
				{

				CString cIGCFilename;
				if( !GetIGCPathName( strArray, pcFlight->m_strFileName, cIGCFilename ) ) 
					{
					pcFlight->SetIGCFileMissing();
					continue;
					}

				// Delete the flight and reread it from the igc file
				RemoveByPointer(pcFlight);
				pcFlight=LoadIGC(cIGCFilename, contestantList );

				if( !pcFlight->ReadFlight() )
					{
					pcFlight->SetIGCFileMissing();
					continue;
					}
				CString cStatus=_T("Analyzing: ")+pcFlight->m_strCID;
				((CMainFrame*)pFrame)->SetStatusText(cStatus);
				pcFlight->m_pDoc=pcView->GetDocument();
				try
					{
					bool bRet=	pcFlight->Analyze(	taskList.GetByDateClass(cDate, eClass), 
													turnpointArray,
													&contestantList,
													eUnits);
					if( !bRet && pcFlight->GetStatus()!=eNoTaskSpecified ) 
						{
						// Report an error, but not if there simply hasn't been a task defined.
						CString cError;
						cError.Format(_T("An error occured processing flight log for %s"),pcFlight->m_strCID);
						AfxMessageBox(cError,MB_ICONSTOP);
						}

					int iWarn=pcFlight->GetWarning(eStartLatest);
					if( iWarn>=0 )
						{
						double dRatio= pcView->GetDocument()->GetLogTurninRatio( pcFlight->GetStartTime(),  pcFlight->m_eClass);
						if( dRatio>.80 ) 
							{
							// If this log had a latest start warning and we have enought logs for a quarum, check for best scoring start.
							if( Rechecklateststart(pcFlight,pcView->GetDocument(), true, false ) )
								{
								pcFlight->RemoveWarning(iWarn);
								}
							}
						}

					} 
				catch(...)
					{
					CString cError;
					cError.Format(_T("Unhandled exception encountered while processing flight log for %s"),pcFlight->m_strCID);
					AfxMessageBox(cError,MB_ICONSTOP);
					}
				pcFlight->FreePositionData();
				pcView->UpdateFlightLogs( pMainFrame, cDate, eClass, pcFlight);
				}
			}
		}

	}



void CFlightList::RemoveByPointer(CFlight *pcRemoveFlight)
{
	CSingleLock cLock(&m_cSem_RemoveByPointer);
	if( IsEmpty() !=0  )  return;
	POSITION thispos, pos = GetHeadPosition();
	while(pos)
		{
		thispos=pos;
		CFlight* pcFlight=GetNext(pos);
		if( pcFlight==pcRemoveFlight ) 
			{
			delete pcFlight;
			RemoveAt(thispos);
			return;
			}
		}
}

CFlight* CFlightList::FindByIGCFileName(CString cIGCFileName )
{
	CSingleLock cLock(&m_cSem_FindByIGCFileName);
	
	cIGCFileName.MakeLower();
	if( IsEmpty() !=0  )  return NULL;
	POSITION thispos, pos = GetHeadPosition();
	while(pos)
		{
		thispos=pos;
		CFlight* pcFlight=GetNext(pos);
		if( !pcFlight ) continue;
		CString strTemp=pcFlight->m_strFileName;
		strTemp.MakeLower();
		if( cIGCFileName.Find(strTemp)>=0 )
			{
			return pcFlight;
			}
		}
	return NULL;
}

bool CFlightList::LoadListFromIGC(CString strPath, CTime cDate, int nDays, CContestantList &contestantList, bool bSilent, CListCtrl *pListCtrl )
	{
	CStringArray strArray;
	bool bAnyAdded=false;
	CWaitCursor cWait;
	CIGCDirectories cIGCDir;

	cIGCDir.FindIGCFiles( cDate, nDays, strArray );

	CTime cmTime;
	CFileFind finder;

	int nRead=strArray.GetSize();

	if( !bSilent )
		{
		CProgressBar cBar(0,nRead, _T("Reading IGC Files"));

		nRead=0;

		for( int iFile=0; iFile<strArray.GetSize(); iFile++ )
			{
			cBar.UpdateProgressBar(nRead++);

			CFlight *pcFlight=FindByIGCFileName( strArray[iFile] );//finder.GetFileName() );
			if( pcFlight!=NULL )
				{
				// One is here with the Same IGC filename.  Check dates
				//CFileStatus rStatus;
				//CFile::GetStatus( strArray[iFile], rStatus ); 
				//finder.GetLastWriteTime(cmTime);
				// Removed the time check below because different computers have different time stamps
				if( /*pcFlight->m_mtime==rStatus.m_mtime &&*/ pcFlight->IsContestant() ) continue;
				RemoveByPointer(pcFlight);
				}

			LoadIGC(strArray[iFile], contestantList);
			bAnyAdded=true;

			}
		}
	else
		{
		nRead=0;
		for( int iFile=0; iFile<strArray.GetSize(); iFile++ )
			{
			CFlight *pcFlight=FindByIGCFileName( strArray[iFile] );
			if( pcFlight!=NULL )
				{
				// One is here with the Same IGC filename.  Check dates
				//finder.GetLastWriteTime(cmTime);
				if( /*pcFlight->m_mtime==cmTime && */ pcFlight->IsContestant() ) continue;
				RemoveByPointer(pcFlight);
				}

			pcFlight=LoadIGC(strArray[iFile], contestantList);
			bAnyAdded=true;
			if( pListCtrl ) 
				{
				pcFlight->AddToList(*pListCtrl );
				}
			}
		}
	return 	bAnyAdded;
	}

CFlight* CFlightList::LoadIGC(CString &strIGCFileName, CContestantList &contestantList )
	{
	CSingleLock cLock(&m_cSem_LoadIGC);
	CContestant *pcContestant;

	CFlight *pcFlight= new CFlight( strIGCFileName );
	if( pcFlight ) 
		{
		AddHead(pcFlight);
		if( !pcFlight->UpdateCID(&contestantList) )
			pcFlight->SetStatus(eNotEnteredInContest);
		else
			{
			pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
			if( pcContestant==NULL )
				pcFlight->SetStatus(eNotEnteredInContest);
			else
				pcFlight->m_eClass=pcContestant->m_eClass;
			}
		}

	return pcFlight;

	}



CFlight* CFlightList::Get(CTime &cDate, CString &strContestNo)
{
	int nMissingENL=0;
	CPtrArray cPtFlights;
	if( IsEmpty() !=0  )  return NULL;
	POSITION pos = GetHeadPosition();
	while(pos)
		{
		CFlight* pcFlight=GetNext(pos);
		if( !pcFlight ) continue;
		if( pcFlight->m_strCID==strContestNo    &&
			pcFlight->m_iDay==cDate.GetDay()	&&
			pcFlight->m_iMonth==cDate.GetMonth()&&
			pcFlight->m_iYear==cDate.GetYear()			) 
			{
			cPtFlights.Add(pcFlight); 
			}
		if( pcFlight->CheckOption(FLT_MISSING_ENL) ) nMissingENL++;
		}
	bool bBadENLLog=nMissingENL<cPtFlights.GetCount();

	if( cPtFlights.GetCount()==1 ) 
		return (CFlight*)cPtFlights.GetAt(0);
	else if( cPtFlights.GetCount()>1 )
		{
//		Duplicate logs, try to pick the "Best Scoring(TM)" one.
//
	//Scan the logs and see which ones were complete and the longest distance.
		int		nComplete=0;
		double	dDistance= -1.0, dBestSpeed=0.0;
		CFlight *pcCompleted=NULL, *pcFlight=NULL, *pcBestSpeed, *pcBestDist=NULL;
		for( int iLog=0; iLog<cPtFlights.GetCount(); iLog++)
			{
			pcFlight=(CFlight*)cPtFlights.GetAt(iLog);
			if( bBadENLLog && pcFlight->CheckOption(FLT_MISSING_ENL) ) continue;
			if( pcFlight->IsTaskComplete() )
				{
				CTimeSpan cSpan=pcFlight->GetFinishTime()-pcFlight->GetStartTime();
				double dHours=(double)cSpan.GetTotalSeconds()/3600;
				double dSpeed=0.0;
				if( dHours>.01 ) dSpeed=pcFlight->GetDistance()/dHours;
				if( dSpeed>dBestSpeed)
					{
					dBestSpeed=dSpeed;
					pcBestSpeed=pcFlight;
					}
				pcCompleted=pcFlight;
				nComplete++;
				}
			if( pcFlight->GetDistance() > dDistance )
				{
				pcBestDist=pcFlight;
				dDistance=pcFlight->GetDistance();
				}
			}
		// If only one flight was completed, return that one
		if( nComplete>0 ) return pcBestSpeed;

		//I give up, just return the longest disance.
		return pcBestDist;
		}
	else
		return NULL;
}

CString CFlightList::GetText(CTime &cDate )
	{
	CString str;
	if( !IsEmpty() )
		{
		POSITION pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos);
			if( pcFlight && 
				pcFlight->m_iDay==cDate.GetDay()	&&
				pcFlight->m_iMonth==cDate.GetMonth()&&
				pcFlight->m_iYear==cDate.GetYear()			) 
				{
				str+=pcFlight->GetFlightText();
				str+=_T("\r\n");
				}
			}
		}
	return str;
	}
/*
CFlightStream* CFlightList::CreateFlightStream( CTurnpointArray &turnpointArray, CContestantList &contestantList, CTime &cDate, int iInterval, int nClasses, EClass aeClasses[], int &nFlights, CProgressCtrl &m_cProgress)
	{
	int nTotalFlights=0;
	nFlights=0;
	if( IsEmpty() ) 
		{
		return NULL;
		}

	CFlight* pcFlight;
	CTime cEarliestTime(0);
	CTime cLatestTime(0);
	CTime cStartFlight;
	int iHomeElevation=0;

	CTurnpoint* pcHome=turnpointArray.GetHomePoint();
	if( pcHome ) 
		iHomeElevation=pcHome->m_iElevation;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcFlight=GetNext(pos,cDate);
		if( pcFlight  )
			{
			CContestant *pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
			if( !pcContestant ) continue;
			if( pcContestant->IsExcludedFromFlightFiles() ) continue;

			bool bInclass=false;
			for( int i=0; i<nClasses; i++ )
				if( pcFlight->m_eClass==aeClasses[i] ) bInclass=true;
			if( !bInclass ) continue;

			if( !pcFlight->ReadFlight() )
				{
				pcFlight->SetIGCFileMissing();
				continue;
				}

			m_cProgress.SetPos( m_cProgress.GetPos()+1);

			CTime cRollTime=pcFlight->GetRollTime();
			CPosition* pcFirstPos=pcFlight->GetPosition(0);
			CTime cFirstPosTime=pcFirstPos->m_cTime;

			if( cRollTime>cFirstPosTime )
				cStartFlight=cRollTime;
			else
				cStartFlight=cFirstPosTime;


			CPosition* pcEndPos=(pcFlight->GetPosition(pcFlight->GetNumPoints()-1));
			CTime cEndFlight=pcEndPos->m_cTime;

			if( cEarliestTime==0 || cEarliestTime>cStartFlight)
				cEarliestTime=cStartFlight;

			if( cLatestTime<cEndFlight )
				cLatestTime=cEndFlight;
			}
		}

	CFlightStream* pcFlightStream= new CFlightStream(cEarliestTime, cLatestTime, iInterval);

	pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcFlight=GetNext(pos,cDate);
		if( pcFlight )
			{
			CContestant *pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
			if( !pcContestant ) continue;
			if( pcContestant->IsExcludedFromFlightFiles() ) continue;

			bool bInclass=false;
			for( int i=0; i<nClasses; i++ )
				if( pcFlight->m_eClass==aeClasses[i] ) bInclass=true;
			if( bInclass )
				{
				pcFlightStream->AddFlight( pcFlight, iHomeElevation );
				pcFlight->FreePositionData();
				nFlights++;
				m_cProgress.SetPos( m_cProgress.GetPos()+1);
				}
			}
		}
	
	return pcFlightStream;

	}
*/
int CFlightList::GetNumberFlights(CTime &cDate, int nClasses, EClass aeClasses[])
{
	int nTotalFlights=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CFlight* pcFlight=GetNext(pos,cDate);
		if( pcFlight && 
			!pcFlight->m_bFileMissing )
			{
			bool bInclass=false;
			for( int i=0; i<nClasses; i++ )
				if( pcFlight->m_eClass==aeClasses[i] ) bInclass=true;
			if( bInclass ) nTotalFlights++;
			}
		}
	return nTotalFlights;
}




void CFlightList::GetXML(CXMLMgr &cMgr, EClass eClass, CTime cDate, MSXML2::IXMLDOMNodePtr &pParentNode, CContestantList &contestantList)
	{

 	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	TESTHR(cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Flights")));

	if( !IsEmpty() ) 
		{
		POSITION pos = GetHeadPosition();
		while( pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos,cDate);
			if( pcFlight  )
				{
				CContestant *pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
				if( pcContestant && 
					!pcContestant->IsExcludedFromFlightFiles() &&
					pcFlight->m_eClass==eClass && 
					!pcFlight->m_bFileMissing) 
					{
			
					MSXML2::IXMLDOMNodePtr pLog;
					cMgr.CreateChild(pIDOMChildNode, pLog, _T("Log"));
					cMgr.CreateElement(pLog, _T("CID"), LPCSTR(pcFlight->m_strCID) );
					cMgr.CreateElement(pLog, _T("IGCFileName"), LPCSTR(pcFlight->m_strFileName));
					}
				}
			}
		}
	}


void CFlightList::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	if( !IsEmpty() ) 
		{
		MSXML2::IXMLDOMNodePtr pIDOMChildNode;
		TESTHR(cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Flights")));

		POSITION pos = GetHeadPosition();
		while( pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos);
			if( pcFlight  )
				{
				pcFlight->GetXML(cMgr, pIDOMChildNode);
				}
			}
		}
	}


int CFlightList::GetFileList(EClass eClass, CTime cDate, CStringArray &strarray, CContestantList &contestantList)
{
	int nAdded=0;
	if( !IsEmpty() ) 
		{
		POSITION pos = GetHeadPosition();
		while( pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos,cDate);
			if( pcFlight &&
				pcFlight->m_eClass==eClass && 
				!pcFlight->m_bFileMissing )
				{
				CContestant *pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
				ASSERT(pcContestant);
				if( pcContestant && 
					!pcContestant->IsExcludedFromFlightFiles() ) 
					{
					strarray.Add(pcFlight->m_strFilePath);
					nAdded++;
					}
				}
			}
		}
	return nAdded;
}

int CFlightList::GetFileList2( EClass eClass, 
							   CTime cDate, 
							   CStringArray &strarrayPath, 
							   CStringArray &strarrayName, 
							   CContestantList &contestantList,
							   CScoreRecordList &scoreRecordList,
							   int iMinDayRank)
{
	int nAdded=0;
	if( !IsEmpty() ) 
		{
		POSITION pos = GetHeadPosition();
		while( pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos,cDate);
			if( pcFlight &&
				pcFlight->m_eClass==eClass && 
				!pcFlight->m_bFileMissing )
				{
				CContestant *pcContestant=contestantList.GetByContestNo(pcFlight->m_strCID);
				ASSERT(pcContestant);
				if( pcContestant && pcContestant->IsExcludedFromFlightFiles() )  continue;

				CScoreRecord *pcRec=scoreRecordList.Get(pcContestant->m_strContestNo, cDate);
				ASSERT(pcRec);
				if( !pcRec ) continue;
				if( pcRec->m_iDayRank>iMinDayRank ) continue;

				strarrayPath.Add(pcFlight->m_strFilePath);
				strarrayName.Add(pcFlight->m_strFileName);
				nAdded++;
				}
			}
		}
	return nAdded;
}

int CFlightList::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pWinscore, CTaskList &cTaskList)
	{
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pFlights;
	TESTHR(cMgr.SelectChild(_T("Flights"),pWinscore, pFlights));
	if( pFlights==NULL ) return 0;

	MSXML2::IXMLDOMNodeListPtr pFlightList=cMgr.SelectNodeList(_T("Flight"),pFlights );
	if( pFlightList==NULL ) return 0;

	long length=0;
	TESTHR(pFlightList->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pFlight;
		TESTHR(pFlightList->get_item(i,&pFlight));
		CFlight* pcFlight=NULL;
		try
			{
			pcFlight=new CFlight( cMgr, pFlight, cTaskList );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcFlight) delete pcFlight;
			continue;
			}
		AddTail(pcFlight);
		nImported++;
		}

	return nImported;
	}

int	CFlightList::GetClassDateCount(CTime cDate, EClass eClass)
	{
	int iCount=0;
	if( !IsEmpty() ) 
		{
		POSITION pos = GetHeadPosition();
		while( pos!=NULL)
			{
			CFlight* pcFlight=GetNext(pos,cDate);
			if( pcFlight &&
				pcFlight->m_eClass==eClass &&
				(pcFlight->GetStatus()==eBelowFinishCylinder||pcFlight->GetStatus()==eIncompleteTask||pcFlight->GetStatus()==eCompletedTask) )
				{
				iCount++;
				}
			}
		}
	return iCount;
	}

void SortNoLogArrays(CStringArray &strArContestNo, CStringArray &strArName, CStringArray &strArStatus, int iSortColumn)
	{
	CString tempStr;
	if( strArStatus.GetCount()<=1 ) return;

	const int size = strArStatus.GetSize();
	BOOL bSwapped = TRUE; // this variable is used to determine whether 
	// there's been any swap in given iteration. If there hasn't, this means that the array is already
	// sorted, so we can exit the loop.

	for (int i = 0; i < size && bSwapped == TRUE; i++)
		{
		bSwapped = FALSE;
		for (int j = 0; j< size-(i+1); j++)
			{
			if (	(iSortColumn==1 && strArContestNo.GetAt(j).Collate(strArContestNo.GetAt(j+1))	< 0)  ||
					(iSortColumn==2 && strArName.GetAt(j).Collate(strArName.GetAt(j+1))				< 0)  ||
					(iSortColumn==3 && strArStatus.GetAt(j).Collate(strArStatus.GetAt(j+1))			< 0) 	)
				{
				// elements [j] and [j + 1] aren't in good order, so let's swap them
				tempStr = strArStatus.GetAt(j);
				strArStatus.SetAt(j, strArStatus.GetAt(j + 1));
				strArStatus.SetAt(j + 1, tempStr);

				tempStr = strArContestNo.GetAt(j);
				strArContestNo.SetAt(j, strArContestNo.GetAt(j + 1));
				strArContestNo.SetAt(j + 1, tempStr);

				tempStr = strArName.GetAt(j);
				strArName.SetAt(j, strArName.GetAt(j + 1));
				strArName.SetAt(j + 1, tempStr);
				bSwapped = TRUE;
				}
			}
		}
	}