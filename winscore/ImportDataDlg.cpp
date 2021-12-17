//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ImportDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreDoc.h"
#include "EventList.h"
#include "TaskList.h"
#include "LandingCardList.h"
#include "Penality.h"
#include "PenalityList.h"
#include "ImportDataDlg.h"
#include <fstream.h>
#include "Global_Prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXLINELEN 256

#define LDG_LOCATION              0x0001
#define LDG_CTL_POINT             0x0002

#define LASTIMPORTEDFILENAME   "LastImportedFile"


#define CLT_READ		0x0001
#define TOT_READ		0x0002
#define STT_READ		0x0004
#define FIT_READ		0x0008
#define LDG_READ		0x0010
#define SLD_READ		0x0020
#define ADJ_READ		0x0040
#define CID_READ		0x0080


#define	CLS_READ		0x0001
#define	TSK_READ		0x0002
#define	TCP_READ		0x0004
#define	STI_READ		0x0008
#define	FDI_READ		0x0010
#define	LTI_READ		0x0020
#define	TST_READ		0x0040
#define	TFI_READ		0x0080
#define TOP_READ		0x0100


CString cErrorText;

#define   FATAL_ERROR(SpecErr)   { \
			     cErrorText.Format( "ERROR occured on line : %d; Field : %s ", nLinesRead, cField ); \
			     m_cListBox.AddString(cErrorText);	\
			     m_cListBox.AddString(SpecErr);	\
				 return;  }  \

#define   ECHO(SpecErr)   { \
			     m_cListBox.AddString(SpecErr);	  }  \

static bool PopField( CString &cRecord, CString &cField, EFieldID* peFieldID, CString &cDataString);
static bool DecodeDate(CString DataString, CTime &cTime );
static bool DecodeClass(CString DataString, EClass &eClass );
static bool DecodeTime(CString DataString, CTime &cCurrentDate, CTime &cTime );
static bool DecodeTimeInterval(CString DataString, CTimeSpan &cTimeSpan );
static bool DecodeLandingInfo(CString DataString, CTime &cCurrentDate, CTime &cLandingTime, int &iLandingData, CLocation &cLocation, int &iLandingCtlID );
static bool DecodeScoredLandingInfo(CString DataString, int &iLandingData, CLocation &cLandingLocation, int &iLandingCtlID );
static bool DecodePointList(CString DataString, int &nPoints, int aiPointList[], int &fSeekingLast );
static bool DecodeTask(CString DataString, ETaskType &eType, CTimeSpan &cMinTaskTime, CTimeSpan &cMaxTaskTime );
static bool DecodeGate(CString DataString, EGateType &eType, double &dRadius );
static bool DecodeAdjustment(CString DataString, EPenality &ePenality, CString &cCode, CString &cReason, int &iPenality );


/////////////////////////////////////////////////////////////////////////////
// CImportDataDlg dialog


CImportDataDlg::CImportDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportDataDlg)
	m_cImportFileName = _T("");
	//}}AFX_DATA_INIT
}


void CImportDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDataDlg)
	DDX_Control(pDX, IDC_LIST, m_cListBox);
	DDX_Control(pDX, IDC_IMPORT_BUTTON, m_cImportButton);
	DDX_Control(pDX, IDOK, m_cAcceptButton);
	DDX_Text(pDX, IDC_FILENAME, m_cImportFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDataDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDataDlg)
	ON_BN_CLICKED(IDOK, OnAccept)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IMPORT_BUTTON, OnImportButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportDataDlg message handlers

BOOL CImportDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cImportFileName=AfxGetApp()->GetProfileString(REGSECTION, LASTIMPORTEDFILENAME );
	m_iOverWrite=IDNO;

	UpdateData(FALSE);
	m_cAcceptButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportDataDlg::OnAccept() 
{

	CString cErr=m_cCurrentDate.Format("Remove *ALL* previous times and landing cards for :  %A, %B %d, %Y?" );
	m_iOverWrite=AfxMessageBox( cErr, MB_YESNOCANCEL  );
	if( m_iOverWrite==IDCANCEL)	return;

	CDialog::OnOK();	
}

void CImportDataDlg::OnBrowse() 
	{
	static char BASED_CODE szFilter[] = "Contest Flight Data Files (*.cfd)|*.cfd|All Files (*.*)|*.*||";


	//Correct for timezone as the above constructor is local.
//	CTime gmt=CTime( 1999, 1, 9, 00, 00, 01)-CTimeSpan(_timezone); //early gmt time

//	CString strlocal = gmt.Format(    "%A, %B %d, %Y, %H:%M:%S " );
//	CString strgmt   = gmt.FormatGmt( "%A, %B %d, %Y, %H:%M:%S " );


	CFileDialog  cFileDlg(TRUE, ".cfd", m_cImportFileName, OFN_OVERWRITEPROMPT, szFilter);
	cFileDlg.m_ofn.lpstrTitle="Select Contest Flight Data File";

	if( cFileDlg.DoModal()==IDOK )
		{
		m_cImportFileName=cFileDlg.GetPathName();
		UpdateData(FALSE);
		}	
	}

void CImportDataDlg::OnImportButton() 
{

UpdateData();

	AfxGetApp()->WriteProfileString(REGSECTION, LASTIMPORTEDFILENAME, m_cImportFileName);

	m_cListBox.ResetContent();

	char  cRecord[MAXLINELEN] ;
	int nLinesRead=0;
	bool fValidDate=FALSE;

	ifstream	cIfstream;

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(m_cImportFileName);
	if (cIfstream.is_open()==0)
		{
		return;
		}

	while( cIfstream.good() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		nLinesRead++;
		if( cIfstream.fail() ) break;
		CString strRecord(cRecord);
		strRecord.TrimLeft();
		if( strRecord.GetLength()<=0 ) continue;

		m_cListBox.AddString(strRecord);

        if( (strRecord[0]=='*' && strRecord[1]=='*') ||
			(strRecord[0]=='/' && strRecord[1]=='/')	) continue;

		EGateType	eStartGateType, eFinishGateType;
		EPenality	ePenality;
		ETaskType	eTaskType;
		EFieldID	eFieldID;
		CString		DataString, cField, cContestNo, cCode, cReason;
		EClass		eCurrentClass;
		CTime		cTaskOpenTime, cStartTime, cFinishTime, cTakeOffTime, cLandingTime;
		CTimeSpan	cFlightDocInterval, cLandingTimeInterval, cMinTaskTime(0), cMaxTaskTime(0);
		CLocation	cLandingLocation, cScoredLandingLocation;
		int			iLandingCtlID=(-1), iLandingData=0, iDum, iPenality=0;
		int			iScoredLandingCtlID=(-1), iScoredLandingData=0;
		int			nClaimedPoints, aiClaimedPoints[100], nTCPoints, aiTCPointList[100];
		int			iSeekingPoint=(-1), iContestantDataRead=0, iTaskDataRead=0;
		double		dStartRadius, dFinishRadius;


		while ( PopField(strRecord, cField, &eFieldID, DataString) )
			{
			switch( eFieldID )
				{
				case eDate:
					{
					if( DecodeDate(DataString, m_cCurrentDate) )
						{
						FATAL_ERROR( "Error reading date" );
						}
					if( !m_pDoc->IsDateValid(m_cCurrentDate) )
						{
						FATAL_ERROR( "File date is not day in the contest." );
						}
					fValidDate=TRUE;
					break;
					}


				case eClass:
					{
					if( DecodeClass(DataString, eCurrentClass) )
						{
						FATAL_ERROR( "Invalid class" );
						}
					iTaskDataRead|=CLS_READ;
					break;
					}


				case eTaskDefinition:
					if( DecodeTask(DataString,  eTaskType, cMinTaskTime, cMaxTaskTime) )
						{
						FATAL_ERROR( "Invalid Task specification." );
						}
					iTaskDataRead|=TSK_READ;
					break;


				case eTaskControlPointList:
					{
					if(  DecodePointList( DataString, nTCPoints, aiTCPointList, iDum) )
						{
						FATAL_ERROR( "Invalid Task Control Point List" );
						}
					iTaskDataRead|=TCP_READ;
					break;
					}
								

				case eTaskStartSpecification:
					if( DecodeGate( DataString, eStartGateType,  dStartRadius) )
						{
						FATAL_ERROR( "Invalid Task Start Specification." );
						}
					iTaskDataRead|=TST_READ;
					break;


				case eTaskFinishSpecification:
					if( DecodeGate( DataString, eFinishGateType,  dFinishRadius) )
						{
						FATAL_ERROR( "Invalid Task Finish Specification." );
						}
					iTaskDataRead|=TFI_READ;
					break;


				case eTaskOpenTime:
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( DecodeTime( DataString, m_cCurrentDate, cTaskOpenTime) )
						{
						FATAL_ERROR( "Invalid Task Open Time." );
						}
					iTaskDataRead|=TOP_READ;
					break;

				case eCompetitionID:
					cContestNo=DataString;
					if( !m_pDoc->IsContestNoValid(cContestNo) )
						{
						ECHO("WARNING - "+cContestNo+" is not entered in the contest." );
						}
					iContestantDataRead|=CID_READ;
					break;


				case eClaimedTask:
					{
					if( DecodePointList(DataString, nClaimedPoints, aiClaimedPoints, iSeekingPoint) )
						{
						FATAL_ERROR( "Invalid Claimed Task." );
						}
					iContestantDataRead|=CLT_READ;
					break;
					}

				case eTakeoffTime:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't Assign Take Off Time witout valid CID Field in the record." );
						}
					if( DecodeTime(DataString, m_cCurrentDate, cTakeOffTime) )
						{
						FATAL_ERROR( "Invalid Take Off Time." );
						}
					if( m_pDoc->IsContestNoValid(cContestNo) )
						{
						m_EventList.AddToList( cContestNo, cTakeOffTime, eRoll );
						iContestantDataRead|=TOT_READ;
						}
					break;
					}


				case eStartTime:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't Assign Start Time witout valid CID Field in the record." );
						}
					if( DecodeTime(DataString, m_cCurrentDate, cStartTime) )
						{
						FATAL_ERROR( "Invalid Start Time." );
						}
					if( m_pDoc->IsContestNoValid(cContestNo) )
						{
						m_EventList.AddToList( cContestNo, cStartTime, eStart );
						iContestantDataRead|=STT_READ;
						}
					break;
					}


				case eFinishTime:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't Assign Finish Time witout valid CID Field in the record." );
						}
					if( DecodeTime(DataString, m_cCurrentDate, cFinishTime) )
						{
						FATAL_ERROR( "Invalid Finish Time." );
						}
					if( m_pDoc->IsContestNoValid(cContestNo) )
						{
						m_EventList.AddToList( cContestNo, cFinishTime, eFinish );
						iContestantDataRead|=FIT_READ;
						}
					break;
					}

				case eLandingInformation:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't Assign Landing Information witout valid CID Field in the record." );
						}
					if( DecodeLandingInfo(DataString, m_cCurrentDate, cLandingTime, iLandingData, cLandingLocation, iLandingCtlID) )
						{
						FATAL_ERROR( "Invalid Landing Information." );
						}
					if( (iLandingData & LDG_CTL_POINT) && !m_pDoc->IsCPIDValid(iLandingCtlID) )
						{
						FATAL_ERROR( "Invalid task control point in Landing Data field." );
						}
					if( m_pDoc->IsContestNoValid(cContestNo) )
						{
						m_EventList.AddToList( cContestNo, cLandingTime, eLanding );
						iContestantDataRead|=LDG_READ;
						}
					break;
					}
				case eScoredLandingInformation:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't Assign Scored Landing Information witout valid CID Field in the record." );
						}
					if( DecodeScoredLandingInfo(DataString, iScoredLandingData, cScoredLandingLocation, iScoredLandingCtlID) )
						{
						FATAL_ERROR( "Invalid Scored Landing Information." );
						}
					if( (iScoredLandingData & LDG_CTL_POINT) && !m_pDoc->IsCPIDValid(iScoredLandingCtlID) )
						{
						FATAL_ERROR( "Invalid task control point in Scored Landing Data field." );
						}
					iContestantDataRead|=SLD_READ;
					break;
					}

				case eScoreAdjustment:
					{
					if( !fValidDate )
						{
						FATAL_ERROR( "ERROR: No Date Record found in import file." );
						}
					if( cContestNo.IsEmpty() ) 
						{
						FATAL_ERROR( "Can't adjust score witout valid CID Field in the record." );
						}

					if( DecodeAdjustment(DataString, ePenality, cCode, cReason, iPenality) )
						{
						FATAL_ERROR( "Invalid Score adjustement." );
						}
					if( m_pDoc->IsContestNoValid(cContestNo) )
						{
						CPenality* pPen=new CPenality(cContestNo, m_cCurrentDate, ePenality, cCode, cReason, iPenality);
						m_PenalityList.AddToList(pPen);
						delete pPen;
						}

					iContestantDataRead|=ADJ_READ;
					break;
					}

				default:
					{
					FATAL_ERROR( "Unrecognized Field" );
					}

				} // end switch on field types
			}// end loop on fields

		//  The record is finished, process landing cards and tasks.
		if( iTaskDataRead!=0 ) 
			{
			if( !(iTaskDataRead & CLS_READ) || 
				!(iTaskDataRead & TSK_READ)    )
				{
				//error
				}

			if( !fValidDate )
				{
				FATAL_ERROR( "ERROR: No Date Record found in import file." );
				}

			// TASK Data in list
			CTask* pcTask=new CTask();
			pcTask->m_cDate	=m_cCurrentDate;
			pcTask->m_eClass=eCurrentClass;
			pcTask->m_eStatus=eUnofficial;

			pcTask->m_eType=eTaskType;	
			pcTask->m_cPostTime=cMinTaskTime;

			if( iTaskDataRead & TCP_READ )
				{
				pcTask->SetNumTurnpoints(nTCPoints);
				for( int i=0; i<nTCPoints; i++ )
					{
					pcTask->SetTurnpointID(i, aiTCPointList[i]);
					}
				}

			if( iTaskDataRead & TOP_READ )
				{
				pcTask->m_cGateOpenTime=cTaskOpenTime;
				}

			if( iTaskDataRead & TST_READ )
				{
				pcTask->m_cStartGate.SetGateType(eStartGateType);
				pcTask->m_cStartGate.SetRadius(ConvertDistance( dStartRadius, eStatute, SYSTEMUNITS ));
				}

			if( iTaskDataRead & TFI_READ )
				{
				pcTask->m_cFinishGate.SetGateType(eFinishGateType);
				pcTask->m_cFinishGate.SetRadius(ConvertDistance( dFinishRadius, eStatute, SYSTEMUNITS ));
				}
			m_TaskList.AddToList(pcTask);
			delete pcTask;

			}



		//  Process landing cards 
		if(((iContestantDataRead & FIT_READ) ||
			(iContestantDataRead & LDG_READ) ||
			(iContestantDataRead & SLD_READ) ||
			(iContestantDataRead & CLT_READ)	)  && m_pDoc->IsContestNoValid(cContestNo) )
			{
			// Some contestant data appeared in the record.  Create
			// a landing card for him.
			if( !fValidDate )
					{
					FATAL_ERROR( "ERROR: No Date Record found in import file." );
					}

			CLandingCard* pcLandingCard=new CLandingCard( cContestNo, m_cCurrentDate );

			if( CLT_READ & iContestantDataRead ) 
				{
				pcLandingCard->m_nTurnpoints=nClaimedPoints;
				for( int i=0; i<nClaimedPoints; i++)
					{
					pcLandingCard->m_aiTurnpoints[i]=aiClaimedPoints[i];
					}
				if( iSeekingPoint>0 ) pcLandingCard->m_iTurnpointSought=iSeekingPoint;
				}


			if( (FIT_READ & iContestantDataRead) )	pcLandingCard->m_fCompleted=true;
			
			if( (SLD_READ & iContestantDataRead) )
				{
				if( iScoredLandingData & LDG_LOCATION )
					{
					pcLandingCard->m_fCompleted=false;
					pcLandingCard->m_cLandoutLocation=cScoredLandingLocation;
					}
				if( iScoredLandingData & LDG_CTL_POINT )
					{
					pcLandingCard->m_fCompleted=false;
					pcLandingCard->m_iLandoutTurnpoint=iScoredLandingCtlID;
					pcLandingCard->m_bDesignatedAirfield=m_pDoc->IsTurnpointAirfield(iScoredLandingCtlID);
					pcLandingCard->m_cLandoutLocation=*(m_pDoc->m_turnpointArray[iScoredLandingCtlID-1]);
					}	
				}
			else if( (LDG_READ & iContestantDataRead) )
				{
				if( iLandingData & LDG_LOCATION )
					{
					pcLandingCard->m_fCompleted=false;
					pcLandingCard->m_cLandoutLocation=cLandingLocation;
					}
				if( iLandingData & LDG_CTL_POINT )
					{
					pcLandingCard->m_fCompleted=false;
					pcLandingCard->m_bDesignatedAirfield=m_pDoc->IsTurnpointAirfield(iLandingCtlID);
					pcLandingCard->m_iLandoutTurnpoint=iLandingCtlID;
					pcLandingCard->m_cLandoutLocation=*(m_pDoc->m_turnpointArray[iLandingCtlID-1]);
					}
				}
				
			//Add Landing Card to List
			m_LandingCardList.AddToList(pcLandingCard);
			delete pcLandingCard;
			}


		}// end loop on records

	cIfstream.close();

	ECHO("==============================================================\n");
	CString cStatus;
	cStatus.Format("Number of Times read: %d", m_EventList.GetCount() );
	ECHO(cStatus);
	cStatus.Format("Number of Landing Cards: %d", m_LandingCardList.GetCount() );
	ECHO(cStatus);
	cStatus.Format("Number of Tasks: %d", m_TaskList.GetCount() );
	ECHO(cStatus);
	cStatus.Format("Number of Adjustements: %d", m_PenalityList.GetCount() );
	ECHO(cStatus);

	m_cAcceptButton.EnableWindow(TRUE);

	return;
	}


static bool DecodeDate(CString DataString, CTime &cTime )
	{
	int iYear, iMonth, iDay;
	char cBuf1[256];
	char *pChar;

	strncpy( cBuf1, LPCTSTR(DataString), 256 );
	pChar=cBuf1;

// 	Read the year
	if( strctok( cBuf1, &pChar, '/' )==0 )
		{
		//error
		return 1;
		}
	if( sscanf(cBuf1, "%d", &iYear)!=1 ) 
		{
		// error
		return 1;
		};

// 	Read the month
	if( strctok( cBuf1, &pChar, '/' )==0 )
		{
		//error
		return 1;
		}
	sscanf(cBuf1, "%d", &iMonth);

	// Read the Day
	if( sscanf(pChar, "%d", &iDay)!=1 ) 
		{
		//error
		return 1;
		};

	// Paranoia Check
	if( iYear<1998 || iMonth<1 || iMonth>12 ||iDay<1 || iDay>31 ) 
		{
		// error
		return 1;
		}

	cTime=CTime( iYear, iMonth, iDay, 0, 0, 0  );

	return 0;

	}




static bool DecodeTime(CString DataString, CTime &cCurrentDate, CTime &cTime )
	{
	int iHour=0, iMin=0, iSec=0;
	int iYear=cCurrentDate.GetYear();
	int iMonth=cCurrentDate.GetMonth();
	int iDay=cCurrentDate.GetDay();

	char cBuf1[256];
	char *pChar;

	strncpy( cBuf1, LPCTSTR(DataString), 256 );
	pChar=cBuf1;

// 	Read the hour
	if( strctok( cBuf1, &pChar, ':' )==0 )
		{
		//error
		return 1;
		}
	if( sscanf(cBuf1, "%d", &iHour)!=1 ) 
		{
		// error
		return 1;
		};

// 	Read the minutes
	if( strctok( cBuf1, &pChar, ':' )==0 )
		{
		//no seconds, just read the minutes
		if( sscanf(pChar, "%d", &iMin)!=1 ) return 1;
		iSec=0;
		}
	else
		{
		// Read the minutes, then seconds
		if( sscanf(cBuf1, "%d", &iMin)!=1 ) return 1;
		if( sscanf(pChar, "%d", &iSec)!=1 ) 
			{
			//error
			iSec=0;
			};
		}

	// Paranoia Check
	if( iHour<0 || iHour>23 || iMin<0 || iMin>59 || iSec<0 || iSec>59 )
		{
		// error
		return 1;
		}

	//Compute the time.  As the CTime constructor only accepts local time, but the flight time is in GMT, we correct this by subtracting this timezone's difference
	cTime=CTime( iYear, iMonth, iDay,  iHour, iMin, iSec, 0 )-CTimeSpan(_timezone);

	//	CString strgmt   = cTime.FormatGmt( "%A, %B %d, %Y, %H:%M:%S " );

	return 0;

	}



static bool DecodeTimeInterval(CString DataString, CTimeSpan &cTimeSpan )
	{
	int iHours=0, iMin=0;

	char cBuf1[256];
	char *pChar;

	strncpy( cBuf1, LPCTSTR(DataString), 256 );
	pChar=cBuf1;

// 	Read the hours
	if( strctok( cBuf1, &pChar, ':' )==0 )
		{
		//error
		return 1;
		}
	if( sscanf(cBuf1, "%d", &iHours)!=1 ) 
		{
		// error
		return 1;
		};

	// Read the minutes, if there
	if( sscanf(pChar, "%d", &iMin)!=1 ) 
		{
		//error
		iMin=0;
		};

	// Paranoia Check
	if( iHours<0 || iHours>23 || iMin<0 || iMin>59  )
		{
		// error
		return 1;
		}

	//Compute the time span
	cTimeSpan=CTimeSpan( 0, iHours, iMin, 0 );
	
	return 0;

	}



static bool DecodeClass(CString DataString, EClass &eClass )
	{

	if( DataString=="OP" )
		eClass=eOpen;
	else if( DataString=="ST" )
		eClass=eStandard;
	else if( DataString=="15" )
		eClass=e15Meter;
	else if( DataString=="18" )
		eClass=e18Meter;
	else if( DataString=="SP" )
		eClass=eSports;
	else if( DataString=="WC" )
		eClass=eWorld;
	else 
		return 1;

	return 0;

	}



static bool PopField( CString &cRecord, CString &cField, EFieldID* peFieldID, CString &cDataString)
	{
	cRecord.TrimRight();
	cRecord.TrimLeft();
	if( cRecord.IsEmpty() ) return false;


	if ( cRecord.Find("//")==0 ) 
		{
		return false;
		}


	cField=cRecord.SpanExcluding(";");

	//  Strip Remove this field from the record.
	int nRemaining=cRecord.GetLength()-cField.GetLength()-1;
	if( nRemaining>0 ) 
		cRecord=cRecord.Right( nRemaining );
	else
		cRecord="";

	cField.TrimRight();
	cField.TrimLeft();

	cDataString=cField.Right(cField.GetLength()-4);

	if		( cField.Find("DTE")==0 )
		*peFieldID=eDate;
	else if ( cField.Find("CLS")==0 )
		*peFieldID=eClass;
	else if ( cField.Find("TSK")==0 )
        *peFieldID=eTaskDefinition;
	else if ( cField.Find("TCP")==0 )
        *peFieldID=eTaskControlPointList;
	else if ( cField.Find("TST")==0 )
        *peFieldID=eTaskStartSpecification;
	else if ( cField.Find("TFI")==0 )
        *peFieldID=eTaskFinishSpecification;
	else if ( cField.Find("CID")==0 )
        *peFieldID=eCompetitionID;
	else if ( cField.Find("CLT")==0 )
        *peFieldID=eClaimedTask;
	else if ( cField.Find("TOT")==0 )
        *peFieldID=eTakeoffTime;
	else if ( cField.Find("STT")==0 || cField.Find("RST")==0 )
        *peFieldID=eStartTime;
	else if ( cField.Find("FIT")==0 )
        *peFieldID=eFinishTime;
	else if ( cField.Find("LDG")==0 )
        *peFieldID=eLandingInformation;
	else if ( cField.Find("SLD")==0 )
        *peFieldID=eScoredLandingInformation; 
	else if ( cField.Find("ADJ")==0 )
        *peFieldID=eScoreAdjustment;
	else if ( cField.Find("TOP")==0 )
        *peFieldID=eTaskOpenTime;
	else
        *peFieldID=eUnknown;

	return true;
	}

static bool DecodeLandingInfo(CString DataString, CTime &cCurrentDate, CTime &cLandingTime, int &iLandingData, CLocation &cLandingLocation, int &iLandingCtlID )
	{
	
	iLandingData=0;

	if( DecodeTime(DataString, cCurrentDate, cLandingTime) ) return 1;

	int iCommaPos=DataString.Find(",");

	if( iCommaPos>0 ) 
		{
		//  There is more to this field, read it.
		CString strOptions=DataString.Right(DataString.GetLength()-iCommaPos-1);


		if( DataString.Find(".")>0 ) 
			{
			// There are lat/longs present, read them
			cLandingLocation=CLocation(strOptions);
			iLandingData=LDG_LOCATION;
			}
		else
			{
			// Only a control Id here, read it.
			if(  sscanf(LPCTSTR(strOptions), "%d", &iLandingCtlID)!=1 ) return 1;
			iLandingData=LDG_CTL_POINT;
			}
		}

	return 0;
	}

static bool DecodeScoredLandingInfo(CString DataString, int &iLandingData, CLocation &cLandingLocation, int &iLandingCtlID )
	{
	
	iLandingData=0;


	if( DataString.Find(".")>0 ) 
			{
			// There are lat/longs present, read them
			cLandingLocation=CLocation(DataString);
			iLandingData=LDG_LOCATION;
			}
	else
			{
			// Only a control Id here, read it.
			if(  sscanf(LPCTSTR(DataString), "%d", &iLandingCtlID)!=1 ) return 1;
			iLandingData=LDG_CTL_POINT;
			}

	return 0;
	}


static bool DecodePointList(CString DataString, int &nPoints, int aiPointList[], int &iSeekingPoint )
	{

	char cBuf1[256];
	char *pChar, *pSeekChr;
	nPoints=0;
	iSeekingPoint= -1;

	strncpy( cBuf1, LPCTSTR(DataString), 256 );
	pChar=cBuf1;

	while(1)
		{

		if( strctok( cBuf1, &pChar, ',' )==0 )
			{
			//end, check for seeking character
			pSeekChr=strstr(pChar,">");
			if ( pSeekChr )
				{
				if ( pSeekChr==pChar )
					{
					// there is only the >## remaining
					if( sscanf(	pSeekChr+1, "%d", &iSeekingPoint)!=1 ) return 1;
					}
				else
					{
					// We have a last turnpoint and a seek character ##>##
					strctok( cBuf1, &pChar, '>' );
					if( sscanf(	cBuf1, "%d", &aiPointList[nPoints++])!=1 ) return 1;
					if( sscanf(	pChar, "%d", &iSeekingPoint)!=1 ) return 1;
					}
				}
			else
				{
				if( sscanf(	pChar, "%d", &aiPointList[nPoints++])!=1 ) return 1;
				}

			break;
			}

		if( sscanf(	cBuf1, "%d", &aiPointList[nPoints++])!=1 ) return 1;
	
		}

	return 0;
	}

static bool DecodeTask(CString DataString, ETaskType &eType, CTimeSpan &cMinTaskTime, CTimeSpan &cMaxTaskTime )
	{

	if(		 DataString[0]=='A' ) 
		{
		eType=eAssigned;
		return 0;
		}
	else if( DataString[0]=='M' )
		eType=eModifiedAssigned;
	else if( DataString[0]=='P' ) 
		eType=ePost;

	int iCommaPos=DataString.Find(",");
	if( iCommaPos<0 ) return 0;

	CString strTimes=DataString.Right(DataString.GetLength()-iCommaPos-1);

	iCommaPos=strTimes.Find(",");
	if( iCommaPos<0 ) 
		{
		// Only Min time
		if(  DecodeTimeInterval(strTimes, cMinTaskTime ) ) return 1; 
		}
	else
		{
		if(  DecodeTimeInterval(strTimes.Left(iCommaPos), cMaxTaskTime ) ) return 1; 
		if(  DecodeTimeInterval(strTimes.Right(strTimes.GetLength()-iCommaPos-1), cMinTaskTime ) ) return 1; 
		}

	return 0;
	}


static bool DecodeGate(CString DataString, EGateType &eType, double &dRadius )
	{

	dRadius=0.0;
	float fRead=0.0;
	if(		 DataString[0]=='L' ) 
		{
		eType=eLine;
		return 0;
		}
	else if( DataString[0]=='C' ) 
		eType=eCylinder;


	int iCommaPos=DataString.Find(",");
	if( iCommaPos<0 ) return 1;

	CString strRadius=DataString.Right(DataString.GetLength()-iCommaPos-1);
	iCommaPos=strRadius.Find(",");
	CString strRead=strRadius.Left(iCommaPos);
	if(  sscanf(LPCTSTR(strRead), "%f", &fRead)!=1 ) return 1;

	dRadius=fRead;
	if( fRead<=0.0 ) return 1;
	
	return 0;
	}
/
static bool DecodeAdjustment(CString DataString, EPenality &ePenality, CString &cCode, CString &cReason, int &iPenality)
	{

	float fRead=0;
	int iCommaPos=DataString.Find(",");
	if( iCommaPos<0 ) return 1;

	cCode=DataString.Right(DataString.GetLength()-iCommaPos-1);
	CString strAmount=DataString.Left(iCommaPos);

	if( strAmount=="D" )
		{
		// Disqualified
		ePenality=eZeroScore;
		iPenality=0;
		}
	else
		{
		// Check for %
		int iPercentPos=DataString.Find("%");
		if( iPercentPos>0 )
			{
			ePenality=eDayPercent;
			if(  sscanf(LPCTSTR(strAmount.Left(iPercentPos)), "%f", &fRead)!=1 ) return 1;
			}
		else
			{
			ePenality=eDayPoint;
			if(  sscanf(LPCTSTR(strAmount), "%f", &fRead)!=1 ) return 1;
			}
		iPenality=(int)fRead;
		}

	if( cCode=="UC" )
		{
		cReason="Unsportsmanlike conduct";
		}
	else if( cCode=="UO" )
		{
		cReason="Unsafe operation";

		}
	else if( cCode=="TP" )
		{
		cReason="Turnpoint penalty";

		}
	else if( cCode=="OW" )
		{
		cReason="Overweight";

		}

	return 0;
	}


