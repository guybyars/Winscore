//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// RealtimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ResizingDialog.h"
#include "RealtimeDlg.h"
#include "fix.h"
#include "RealtimeInfoDlg.h"
#include "global_prototypes.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDlg dialog


CRealtimeDlg::CRealtimeDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CRealtimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealtimeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


    SetControlInfo(IDC_WSVIEWCTRL1,		RESIZE_HOR | RESIZE_VER);
//    SetControlInfo(IDC_SLIDER,			RESIZE_HOR | ANCHORE_BOTTOM );
 	SetControlInfo(IDCANCEL,			ANCHORE_RIGHT|ANCHORE_BOTTOM );
 	SetControlInfo(IDC_INFO,			ANCHORE_BOTTOM );
 	SetControlInfo(IDC_REFRESH,			ANCHORE_LEFT|ANCHORE_BOTTOM );
}


void CRealtimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealtimeDlg)
	DDX_Control(pDX, IDC_WSVIEWCTRL1, m_cWSView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealtimeDlg, CDialog)
	//{{AFX_MSG_MAP(CRealtimeDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_INFO, OnInfo)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDlg message handlers


void CRealtimeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);	
}

void CRealtimeDlg::OnPaint() 
{
	 CPaintDC dc(this); // device context for painting		
	 CResizingDialog::OnPaint();
}


BOOL CRealtimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	SetWindowText(cWinText);

	float fTurnpointRadius		= (float)GetProfileDouble(REGSECTION, INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	float fTurnpointOuterRadius	= (float)GetProfileDouble(REGSECTION, OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );
	m_cWSView.SetDisplayOptionD(WSO_INNER_RADIUS, fTurnpointRadius);
	m_cWSView.SetDisplayOptionD(WSO_OUTER_RADIUS, fTurnpointOuterRadius);
	m_cWSView.SetSceneryPath("Maps");

	if( m_pcTask)
		m_pcTask->AddToControl(m_pDoc->m_turnpointArray, m_cWSView);

	// Add the control points
	m_pDoc->m_turnpointArray.AddToControl(m_cWSView);

	m_cWSView.SetDisplayOption(1,0);

	Refresh();

	m_cWSView.Autoscale();

	int iRefreshRate		=AfxGetApp()->GetProfileInt( REGSECTION, REFRESHRATE, 60 ); 

	SetTimer(0, iRefreshRate*1000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealtimeDlg::OnInfo() 
{
	// TODO: Add your control notification handler code here
	
}

void CRealtimeDlg::OnRefresh() 
{
Refresh();	
}

void CRealtimeDlg::Refresh()
	{
	CString str;
	int iFix=0;
	long lEndTime=0;
	long lStartTime=0;
	bool bFirst=true;
	CFixList cFixList;
	CTime tnow = CTime::GetCurrentTime();

	COleSafeArray olesafearrayTime;
	COleSafeArray olesafearrayCID;
	COleSafeArray olesafearrayLat;
	COleSafeArray olesafearrayLong;
	COleSafeArray olesafearrayAlt;

	CStringArray cPilotArray;

	float *pafLat	= NULL;
	float *pafLong	= NULL;
	short *pasAlt	= NULL;
	long *palTime	= NULL;
	char  *pacCID	= NULL;

	ReadFixes( cFixList, str);

	m_cWSView.ResetContestants();

	if( !cFixList.IsEmpty()  )
		{

		int nFixes=cFixList.GetCount();
		olesafearrayTime.CreateOneDim(VT_I4, nFixes);
		olesafearrayCID.CreateOneDim(VT_R4, nFixes*3);
		olesafearrayLat.CreateOneDim(VT_R4, nFixes);
		olesafearrayLong.CreateOneDim(VT_R4, nFixes);
		olesafearrayAlt.CreateOneDim(VT_I2, nFixes);

		olesafearrayTime.AccessData((void **)&palTime);
		olesafearrayCID.AccessData((void **)&pacCID);
		olesafearrayLat.AccessData((void **)&pafLat);
		olesafearrayLong.AccessData((void **)&pafLong);
		olesafearrayAlt.AccessData((void **)&pasAlt);

		int iFix=0;
		POSITION pos = cFixList.GetHeadPosition();
		while(pos!=NULL)
			{
			CFix* pcFix=(CFix*)cFixList.GetNext(pos);
			if( !pcFix ) continue;

			CContestant *pcContestant=m_pDoc->m_contestantList.GetByContestNo(pcFix->m_strContestNo);
			if( !pcContestant ) continue;

			int iPID= -1;
			for( int i=0; i<cPilotArray.GetSize(); i++ )
				{
				if( pcFix->m_strContestNo==cPilotArray[i] )
					{
					iPID=i;
					break;
					}
				}

			if( iPID<0 )
				{
				iPID=m_cWSView.AddPilot(pcFix->m_strContestNo, pcContestant->m_strLastName,pcContestant->m_strFirstName, 0); 
				cPilotArray.SetAtGrow( iPID, pcFix->m_strContestNo );
				}

			pafLat[iFix]	=pcFix->m_fLatitude; 
			pafLong[iFix]	=pcFix->m_fLongitude;
			pasAlt[iFix]	=pcFix->m_iAltitude;
			palTime[iFix]	=pcFix->m_cTime.GetTime();

			if( bFirst )
				{
				bFirst=false;
				lStartTime=pcFix->m_cTime.GetTime();
				}
			else
				lStartTime=min(lStartTime, pcFix->m_cTime.GetTime() );

			lEndTime=max(lEndTime, pcFix->m_cTime.GetTime());

			strcpy( &pacCID[iFix*3], "   ");
			strncpy( &pacCID[iFix*3],  LPCTSTR (pcFix->m_strContestNo), pcFix->m_strContestNo.GetLength() );
			iFix++;
			}

		if( iFix>0 )
			{
			m_cWSView.AddFixes(	olesafearrayTime, 
								olesafearrayCID, 
								olesafearrayLat, 
								olesafearrayLong, 
								olesafearrayAlt);
			}

		olesafearrayTime.UnaccessData();
		olesafearrayCID.UnaccessData();
		olesafearrayLat.UnaccessData();
		olesafearrayLong.UnaccessData();
		olesafearrayAlt.UnaccessData();

		// Now set the colors for the pilots based on the last time you heard from them
		for( int i=0; i<cPilotArray.GetSize(); i++ )
			{
			long lLastPilotTime=0;
			POSITION pos = cFixList.GetHeadPosition();
			while(pos!=NULL)
				{
				CFix* pcFix=(CFix*)cFixList.GetNext(pos);
				if( !pcFix ) continue;
				if( pcFix->m_strContestNo!=cPilotArray[i] ) continue;

				lLastPilotTime=max(lLastPilotTime, pcFix->m_cTime.GetTime() );
				}
			CTimeSpan cSpan=tnow-CTime(lLastPilotTime);
			int nMinutes=cSpan.GetTotalMinutes();

			CString strLastReport;
			strLastReport.Format("%d minutes since last report.", nMinutes);

			m_cWSView.SetPilotText(cPilotArray[i], strLastReport);
			if( nMinutes<=15 )
				{
				m_cWSView.SetPilotTraceColor(cPilotArray[i], RGB(0,255,0) );
				}
			else if( nMinutes<=30 )
				m_cWSView.SetPilotTraceColor(cPilotArray[i], RGB(0,0,255) );
			else if( nMinutes<=60 )
				m_cWSView.SetPilotTraceColor(cPilotArray[i], RGB(255,0,0) );
			else if( nMinutes<=120 )
				m_cWSView.SetPilotTraceColor(cPilotArray[i], RGB(0,0,0) );
			else
				m_cWSView.SetPilotTraceColor(cPilotArray[i], RGB(125,125,125) );

			}




		}
	
	m_cWSView.SetTimeRange(lStartTime, lEndTime);
	m_cWSView.SetCurrentTime(lEndTime);

	CTime cTime(lEndTime);
	CString str1;
	str1=cTime.Format("Last update received at : %H : %M : %S" );

	if( !cFixList.IsEmpty()  )
		SetWindowText( str1 );
	else
		SetWindowText( "No fixes in: "+str );

}


void CRealtimeDlg::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent==0 ) 
		Refresh();

	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CRealtimeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRealtimeDlg)
	ON_EVENT(CRealtimeDlg, IDC_WSVIEWCTRL1, 1 /* PilotSelection */, OnPilotSelection, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CRealtimeDlg::OnPilotSelection(LPCTSTR strContestNo) 
{
	CFixList	cFixList;
	CString str;

	ReadFixes( cFixList, str);

	CRealtimeInfoDlg dlg( cFixList, strContestNo);

	dlg.DoModal();

	return;
}


void CRealtimeDlg::ReadFixes( CFixList &cFixList, CString &str)
	{
	CString strAlphabet="123456789abcdefghijklmnopqrstuv";

	double dYear=((double)m_cDate.GetYear())*.1;
	double dSDYear;
	double dInt=modf( dYear, &dSDYear );
	dSDYear=dInt*10.000001;

	int iMonth=m_cDate.GetMonth()-1;
	int iDay=m_cDate.GetDay()-1;
	CString cMonth=strAlphabet[iMonth];
	CString cDay  =strAlphabet[iDay];

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, REALTIMEPATH);

	str.Format("%s\\%d%s%s*.ppm", strPath, (int)dSDYear, cMonth, cDay );
//	str.Format("%s\\017*.ppm", strPath, (int)dSDYear, cMonth, cDay );

	CTime cmTime;
	CFileFind finder;

	BOOL bWorking = finder.FindFile(str );
	while (bWorking)
		{
		bWorking = finder.FindNextFile();

		cFixList.ReadRealtime( m_cDate.GetYear(), iMonth, iDay, finder.GetFilePath() );
		}
	}



