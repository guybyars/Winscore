//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"

#include "winscore.h"
#include "global_prototypes.h"

#include "flight.h"
#include "progressbar.h"

#include "ResizingDialog.h"
#include "FlightAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlightAnalysisDlg dialog


CFlightAnalysisDlg::CFlightAnalysisDlg(EUnits eUnits, EAltUnits eAltUnits, CWnd* pParent /*=NULL*/)
	: CResizingDialog(CFlightAnalysisDlg::IDD, pParent),
	m_eUnits(eUnits),
	m_eAltUnits(eAltUnits)
{

    SetControlInfo(IDC_LIST2,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDC_PREV_TURNPOINT, ANCHORE_BOTTOM );
    SetControlInfo(IDC_NEXT_TURNPOINT,  ANCHORE_BOTTOM );
    SetControlInfo(IDC_FIND_START,  ANCHORE_BOTTOM );
    SetControlInfo(IDC_FIND_FINISH,  ANCHORE_BOTTOM );
    SetControlInfo(IDC_LANDING,  ANCHORE_BOTTOM );
    SetControlInfo(IDC_PEV,  ANCHORE_BOTTOM );
	SetControlInfo(IDOK,  ANCHORE_BOTTOM );

	m_pcTask=NULL;

	m_iStartItem = -1;
	m_iFinishItem = -1;
	m_iPEVPtr=0;

	//{{AFX_DATA_INIT(CFlightAnalysisDlg)
	//}}AFX_DATA_INIT
}


void CFlightAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlightAnalysisDlg)
	DDX_Control(pDX, IDC_PREV_TURNPOINT, m_cPrevButton);
	DDX_Control(pDX, IDC_NEXT_TURNPOINT, m_cNextButton);
	DDX_Control(pDX, IDC_FIND_START, m_cStartButton);
	DDX_Control(pDX, IDC_FIND_FINISH, m_cFinishButton);
	DDX_Control(pDX, IDC_LIST2, m_cListCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PEV, m_cPEVButton);
}


BEGIN_MESSAGE_MAP(CFlightAnalysisDlg, CDialog)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//{{AFX_MSG_MAP(CFlightAnalysisDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_FIND_FINISH, OnFindFinish)
	ON_BN_CLICKED(IDC_FIND_START, OnFindStart)
	ON_BN_CLICKED(IDC_NEXT_TURNPOINT, OnNextTurnpoint)
	ON_BN_CLICKED(IDC_PREV_TURNPOINT, OnPrevTurnpoint)
	ON_BN_CLICKED(IDC_LANDING, OnLanding)
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU() 
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PEV, &CFlightAnalysisDlg::OnBnClickedPEV)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlightAnalysisDlg message handlers

BOOL CFlightAnalysisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cPrevButton.EnableWindow(FALSE);
	m_cNextButton.EnableWindow(FALSE); 
	m_cStartButton.EnableWindow(FALSE);
	m_cFinishButton.EnableWindow(FALSE);
	m_cPEVButton.EnableWindow(FALSE);

	m_cListCtrl.SetRedraw(FALSE);
	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	bool bStarted=false;
	bool bFinished=false;

	m_iLandingItem= -1;
	m_iStartItem = -1;
	m_iFinishItem = -1;
	for( int i=0; i<MAXTASKTPS; i++ ) m_aiTurnpointItems[i];
	m_nTPItems=0;
	m_iTPItemPtr= -1;
	
	CreateControlColumns(m_cListCtrl);

	CPosition* pcPrevPos=NULL;

	CString cWinText="Flight Analysis for ";
	cWinText+=m_pcFlight->m_strCID+" - "+m_pcFlight->m_strPilot;

	SetWindowText(cWinText);

	int itm=0;
	
	m_cListCtrl.SetItemCount( m_pcFlight->GetNumPoints() );

	CProgressBar cBar(0, m_pcFlight->GetNumPoints(), "Loading View..." );
	CWaitCursor cCursor;

	for ( int iPoint=0; iPoint<m_pcFlight->GetNumPoints(); iPoint++)
		{
		cBar.UpdateProgressBar(iPoint);

		CPosition* pcPos=m_pcFlight->GetPosition(iPoint);

		LV_ITEM lvi;

		lvi.iSubItem = 0;
		lvi.mask = 0;
		int i=0;
		if( pcPos && pcPrevPos && 
			pcPrevPos->m_cTime <=m_pcFlight->GetStartTime()	&& m_pcFlight->GetStartTime()<pcPos->m_cTime   )
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***") );
			m_cListCtrl.SetItemText(iItem,i++,m_pcFlight->GetStartTimeText() );
			m_cListCtrl.SetItemText(iItem,i++,(m_pcFlight->m_cStartGate.IsGPSCylinder())?(_T(" *** START ***")):(_T(" *** CROSSED START LINE ***")));
			m_iStartItem = lvi.iItem;
			m_cStartButton.EnableWindow();
			bStarted=true;
			}
		if( pcPos && pcPrevPos && 
			pcPrevPos->m_cTime < m_pcFlight->GetFinishTime()	&&
			pcPos->m_cTime	   >= m_pcFlight->GetFinishTime()		)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,m_pcFlight->GetFinishTimeText() );
			m_cListCtrl.SetItemText(iItem,i++,(m_pcFlight->m_cFinishGate.IsGPSCylinder())?(_T(" *** FINISHED ***")):(_T(" *** CROSSED FINISH LINE ***")));
			m_iFinishItem =lvi.iItem;
			m_cFinishButton.EnableWindow();
			bFinished=true;
			}
		if( pcPos && pcPrevPos && 
			pcPrevPos->m_cTime < m_pcFlight->GetLandingTime()	&&
			pcPos->m_cTime	   >= m_pcFlight->GetLandingTime()		)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,m_pcFlight->GetLandingTimeText() );
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** LANDED ***"));
			m_iLandingItem=lvi.iItem;
			}
		if( pcPos->CheckStatus(FAN_TURN_AREA_TURNPOINT) )
			{
			m_aiTurnpointItems[m_nTPItems++]=lvi.iItem+1;
			}

		if( pcPos->CheckStatus(FAN_AT_TASKPOINT) )
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,_T(""));
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** Arrived at Turnpoint ***"));
			if(m_pcTask && !m_pcTask->IsAreaTask() )
				m_aiTurnpointItems[m_nTPItems++]=lvi.iItem;

			m_cNextButton.EnableWindow();
			}

		if( pcPos->CheckStatus(FAN_GAP) 	)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,_T(""));
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** GAP ***"));
			}
		if( pcPos->CheckStatus(FAN_FURTHEST_PROGRESS) 	)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,_T(""));
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** FURTHEST PROGRESS ***"));
			}
		if( pcPos && pcPrevPos && 
			pcPrevPos->m_cTime < m_pcFlight->GetTaskOpenTime()	&&
			pcPos->m_cTime	   >= m_pcFlight->GetTaskOpenTime()		)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,m_pcFlight->GetTaskOpenTimeText() );
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** TASK NOW OPEN ***"));
			}
		if( pcPos && pcPrevPos && 
			pcPrevPos->m_cTime <= m_pcFlight->GetRollTime()	&&
			pcPos->m_cTime	   > m_pcFlight->GetRollTime()		)
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,m_pcFlight->GetRollTimeText() );
			m_cListCtrl.SetItemText(iItem,i++,_T(" *** ROLLED ***"));
			}
		if( pcPos && pcPos->m_bPEV )
			{
			i=0;
			lvi.iItem=itm++; 
			int iItem=m_cListCtrl.InsertItem(&lvi);
			CString cInt;
			cInt.Format(_T("%d"), iPoint+1 );
			m_cListCtrl.SetItemText(iItem,i++,_T("***"));
			m_cListCtrl.SetItemText(iItem,i++,pcPos->GetTimeString() );
			if( m_pcFlight->GetPEVStartTime() == pcPos->m_cTime ) 
				m_cListCtrl.SetItemText(iItem,i++,_T(" *** PILOT EVENT START ***"));
			else
				m_cListCtrl.SetItemText(iItem,i++,_T(" *** PILOT EVENT ***"));
			m_vPEVPos.push_back(lvi.iItem);
			m_cPEVButton.EnableWindow();
			}

		if( m_pcTask &&
			m_pcTask->m_cStartGate.IsPEVStart() &&
			pcPos && pcPrevPos )
			{
			CTime cOpenTime=m_pcFlight->GetPEVStartTime()+m_pcTask->m_cStartGate.GetPEVWaitTimeSec();
			if( pcPrevPos->m_cTime<cOpenTime && pcPos->m_cTime>=cOpenTime )
				{
				// PEV Window opens
				i=0;
				lvi.iItem=itm++; 
				int iItem=m_cListCtrl.InsertItem(&lvi);
				CString cInt;
				cInt.Format(_T("%d"), iPoint+1 );
				m_cListCtrl.SetItemText(iItem,i++,_T("***"));
				m_cListCtrl.SetItemText(iItem,i++,cOpenTime.Format( _T("%H:%M:%S"))  );
				m_cListCtrl.SetItemText(iItem,i++,_T(" ***PEV START WINDOW OPEN ***"));
				m_vPEVPos.push_back(lvi.iItem);
				m_cPEVButton.EnableWindow();
				}
			}
		if( m_pcTask &&
			pcPos && pcPrevPos &&
			m_pcTask->m_cStartGate.IsPEVStart() )
			{
			CTime cCloseTime=m_pcFlight->GetPEVStartTime()+m_pcTask->m_cStartGate.GetPEVWaitTimeSec()+m_pcTask->m_cStartGate.GetPEVStartWindowSec();
			if( pcPrevPos->m_cTime<cCloseTime && pcPos->m_cTime>=cCloseTime )
				// PEV Window CLOSES
				{
				i=0;
				lvi.iItem=itm++; 
				int iItem=m_cListCtrl.InsertItem(&lvi);
				CString cInt;
				cInt.Format(_T("%d"), iPoint+1 );
				m_cListCtrl.SetItemText(iItem,i++,_T("***"));
				m_cListCtrl.SetItemText(iItem,i++,cCloseTime.Format( _T("%H:%M:%S")) );
				m_cListCtrl.SetItemText(iItem,i++,_T(" *** PEV START WINDOW CLOSE ***"));
				m_vPEVPos.push_back(lvi.iItem);
				m_cPEVButton.EnableWindow();
				}
			}

		/*
	_T("Point"), 
	_T("Time"), 
	_T("Status"),
	_T("Speed"),
	_T("Altitude"),
	_T("Accuracy"),
	_T("ENL"),
	_T("MOP"),
	_T("ID Turnpoint"),
	_T("Turnpoint Dist"),
	_T("Start Dist."),
	_T("Finish Dist")
	_T("Location"), 
*/
		CString cInt;
		i=0;
		lvi.iItem=itm++; 
		int iItem=m_cListCtrl.InsertItem(&lvi);
		cInt.Format(_T("%d"), iPoint+1 );
		m_cListCtrl.SetItemText(iItem,i++,cInt );
		m_cListCtrl.SetItemText(iItem,i++,pcPos->m_cTime.Format(_T("%H:%M:%S")) );

		m_cListCtrl.SetItemText(iItem,i++,pcPos->GetStatusText(m_pcFlight->m_cStartGate.IsGPSCylinder(), m_pcFlight->m_cStartGate.IsGPSCylinder()) );

		cInt.Format(_T("%6.2lf"), ConvertDistance(pcPos->m_dSpeed, SYSTEMUNITS, m_eUnits) );
		m_cListCtrl.SetItemText(iItem,i++,cInt );

		if( m_eUnits==eKilometers )
			{
			float dAlt=(float)pcPos->m_iGPSAltitude ;
			cInt.Format(_T("%4.1lf"), dAlt/m_to_ft );
			}
		else
			{
			cInt.Format(_T("%5d"), pcPos->m_iGPSAltitude );
			}
		 m_cListCtrl.SetItemText(iItem,i++,cInt );


		if( m_eUnits==eKilometers )
			{
			float dAlt=(float)pcPos->m_iPressureAltitude;
			cInt.Format(_T("%4.1lf"), dAlt/m_to_ft );
			}
		else
			{
			cInt.Format(_T("%5d"), pcPos->m_iPressureAltitude );
			}
		 m_cListCtrl.SetItemText(iItem,i++,cInt );

		if( m_eUnits==eKilometers )
			{
			float dAlt=(float)pcPos->m_iCorrectedAltitude;
			cInt.Format(_T("%4.1lf"), dAlt/m_to_ft );
			}
		else
			{
			cInt.Format(_T("%5d"), pcPos->m_iCorrectedAltitude );
			}
		if( m_pcFlight->IsFinishTimeValid() && pcPos->m_cTime>m_pcFlight->GetFinishTime()-CTimeSpan(0,0,0,60))
			{
			CString strTemp;
			// Added Finish corrected height
			if( m_eUnits==eKilometers )
				{
				float dAlt=(float)pcPos->m_iCorrectedFinishAltitude;
				strTemp.Format(_T(" / %4.1lf"), dAlt/m_to_ft );
				}
			else
				{
				strTemp.Format(_T(" / %5d"), pcPos->m_iCorrectedFinishAltitude );
				}
			cInt+=strTemp;
			}



		if( pcPos->CheckStatus(FAN_ABOVE_START_CYL) ) 
			{
			if( !bStarted ||
			    (pcPos->CheckStatus(FAN_IN_START_CYL) && bStarted) )cInt+=_T(" as");
			}

		if( pcPos->CheckStatus(FAN_IN_FINISH_CYL) && pcPos->CheckStatus(FAN_BELOW_FINISH_CYL) &&	bStarted	) cInt+=_T(" bf");
		 m_cListCtrl.SetItemText(iItem,i++,cInt );


		cInt=_T("");
		if( m_pcFlight->GetHomePoint() ) 
			{
			if( m_eUnits==eKilometers )
				{
				float dAlt=(float)pcPos->m_iCorrectedAltitude-m_pcFlight->GetHomePoint()->m_iElevation;
				cInt.Format(_T("%4.1lf"), dAlt/m_to_ft );
				}
			else
				{
				cInt.Format(_T("%5d"), pcPos->m_iCorrectedAltitude-m_pcFlight->GetHomePoint()->m_iElevation );
				}
			}
		 m_cListCtrl.SetItemText(iItem,i++,cInt );



		cInt.Format(_T("%6.2lf"), ConvertDistance(pcPos->m_dAccuracy, SYSTEMUNITS, m_eUnits) );
		m_cListCtrl.SetItemText(iItem,i++,cInt );

		cInt.Format(_T("%5d"), pcPos->m_iEngineNoiseLevelRAW );
		m_cListCtrl.SetItemText(iItem,i++,cInt );
		cInt.Format(_T("%5d"), pcPos->m_iMOPLevelRAW );
		m_cListCtrl.SetItemText(iItem,i++,cInt );

		m_cListCtrl.SetItemText(iItem,i++,pcPos->GetTurnpointText() );
		m_cListCtrl.SetItemText(iItem,i++, pcPos-> GetTurnpointDistText( m_eUnits ) );
		cInt.Format(_T("%6.3lf"), ConvertDistance(pcPos->m_dDistanceFromStart, SYSTEMUNITS, m_eUnits) );
		m_cListCtrl.SetItemText(iItem,i++,cInt );
		cInt.Format(_T("%6.3lf"), ConvertDistance(pcPos->m_dDistanceFromFinish, SYSTEMUNITS, m_eUnits) );
		m_cListCtrl.SetItemText(iItem,i++,cInt );
		pcPrevPos=pcPos;

		m_cListCtrl.SetItemText(iItem,i++,pcPos->GetFormattedInfo() );

		}
	m_cListCtrl.SetRedraw(TRUE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define NUM_COLUMNS 16

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
	{
	_T("Point"), 
	_T("Time"), 
	_T("Status"),
	_T("Speed"),
	_T("GPS Alt"),
	_T("Press. Alt"),
	_T("Corrected Alt"),
	_T("Alt Above Home Field"),
	_T("Accuracy"),
	_T("ENL"),
	_T("MOP"),
	_T("ID Turnpoint"),
	_T("Turnpoint Dist"),
	_T("Start Dist."),
	_T("Finish Dist"),
	_T("Location") 
	};


static int _gnColumnWidth[NUM_COLUMNS] = 
{
	50, //point
	80, //time
	200, //Status
	60, //speed
	75, //altitude
	75, //altitude
	95, //altitude
	95, //altitude
	75, //accuracy
	75, //ENL
	75, //MOP
	75, //ID tp
	75, //tp dist
	65, //start dist
	65, //finish dist
	195 //location
};

void CFlightAnalysisDlg::CreateControlColumns(CListCtrl& ListCtrl)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		CString str=_gszColumnLabel[i];
		if( i==3 ) 
			{
			// Speed
			str+=UnitsSpeedTextShort(m_eUnits);
			}
		else if( i==11 || i==12  )
			{
			// distance
			str+=_T(" (")+UnitsTextShort(m_eUnits)+_T(")");
			}
		else if( i==4 || i==5 || i==6 || i==7  )
			{
			str+=_T(" (")+AltitudeUnitsText(m_eAltUnits)+_T(")");
			}

		lvc.pszText = (TCHAR*)LPCTSTR(str);
		lvc.cx =  _gnColumnWidth[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

}

void CFlightAnalysisDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	 CResizingDialog::OnPaint();
}

void CFlightAnalysisDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CFlightAnalysisDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowWindow( SW_SHOWMAXIMIZED  );	
	
}

void CFlightAnalysisDlg::OnFindFinish() 
{
	if( m_iFinishItem>0 )
		{
		EnsureVisibleCenter( m_iFinishItem );
		m_cListCtrl.SetItemState( m_iFinishItem, LVIS_SELECTED, LVIS_SELECTED);
		m_cListCtrl.Update( m_iFinishItem );
		m_iTPItemPtr= m_nTPItems-1;
		m_cNextButton.EnableWindow(FALSE);
		m_cPrevButton.EnableWindow();
		}	
}	


void CFlightAnalysisDlg::OnFindStart() 
{
	if( m_iStartItem>0 )
		{
		m_cListCtrl.SetFocus( );
		m_cListCtrl.SetItemState( m_iStartItem, LVIS_SELECTED, LVIS_SELECTED);
		EnsureVisibleCenter( m_iStartItem );

		m_iTPItemPtr= -1;
		m_cNextButton.EnableWindow();
		m_cPrevButton.EnableWindow(FALSE);
		}	
}

void CFlightAnalysisDlg::OnNextTurnpoint() 
{
	m_iTPItemPtr++;
	m_iTPItemPtr=min( m_iTPItemPtr, m_nTPItems-1 );
	if( m_aiTurnpointItems[m_iTPItemPtr] > 0 )
		{
		m_cListCtrl.SetFocus( );
		EnsureVisibleTop(  m_aiTurnpointItems[m_iTPItemPtr] );
		m_cListCtrl.SetItemState(  m_aiTurnpointItems[m_iTPItemPtr], LVIS_SELECTED, LVIS_SELECTED);
		}
	if( m_iTPItemPtr == m_nTPItems-1)
		{
		m_cNextButton.EnableWindow(FALSE);
		}
	m_cPrevButton.EnableWindow();

}

void CFlightAnalysisDlg::OnPrevTurnpoint() 
{
	m_iTPItemPtr--;
	m_iTPItemPtr=max(m_iTPItemPtr,0);
	if( m_aiTurnpointItems[m_iTPItemPtr] > 0 )
		{
		m_cListCtrl.SetFocus( );
		EnsureVisibleTop(  m_aiTurnpointItems[m_iTPItemPtr] );
		m_cListCtrl.SetItemState(  m_aiTurnpointItems[m_iTPItemPtr], LVIS_SELECTED, LVIS_SELECTED);
		}	
	if( m_iTPItemPtr == 0)
		{
		m_cPrevButton.EnableWindow(FALSE);
		}
	m_cNextButton.EnableWindow();
	
}

void CFlightAnalysisDlg::OnLanding() 
{
	if( m_iLandingItem>0 )
		{
		m_cListCtrl.SetFocus( );
		EnsureVisibleCenter( m_iLandingItem );
		m_cListCtrl.SetItemState( m_iLandingItem, LVIS_SELECTED, LVIS_SELECTED);
		}		
}

void CFlightAnalysisDlg::OnClose() 
{
	OnOK();
}

void CFlightAnalysisDlg::OnOK() 
{
	CWaitCursor cWait;
	
	m_cListCtrl.SetRedraw(FALSE);

  //  int iTotal=m_cListCtrl.GetItemCount();
  //  if( iTotal>2000 )
	 //  {
		//CWinApp* theApp = AfxGetApp();
		//CFrameWnd* pcwndMainFrame = (CFrameWnd *) theApp->m_pMainWnd;

		//ShowWindow( SW_HIDE     );
		//pcwndMainFrame->ShowWindow( SW_SHOW  );
		//pcwndMainFrame->SetActiveWindow();
		//pcwndMainFrame->UpdateWindow();

		//CProgressBar cBar(0, iTotal, _T("Closing View...") );

		//for( int i=iTotal-1; i>0; i-- )
		//		{
		//		m_cListCtrl.DeleteItem(i);
		//		if( ((iTotal-i)%1000)==0 )
		//				cBar.UpdateProgressBar(iTotal-i);
		//		}
  //  
	 //  }
    CDialog::OnOK();
}


void CFlightAnalysisDlg::EnsureVisibleCenter( int iItem )
	{
	m_cListCtrl.EnsureVisible( iItem+(m_cListCtrl.GetCountPerPage()/2) , FALSE);
	m_cListCtrl.EnsureVisible( iItem-(m_cListCtrl.GetCountPerPage()/2), FALSE);
	m_cListCtrl.EnsureVisible( iItem, FALSE);
	}

void CFlightAnalysisDlg::EnsureVisibleTop( int iItem )
	{
	m_cListCtrl.EnsureVisible( iItem+(m_cListCtrl.GetCountPerPage()) , FALSE);
	m_cListCtrl.EnsureVisible( iItem-1, FALSE);
	}


void CFlightAnalysisDlg::OnContextMenu(CWnd* pWnd, CPoint point)
	{

	CMenu menu;
	if (menu.LoadMenu(IDR_COPY_ANALYSIS))
		{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
								   point.x,
								   point.y,
								   this ); 
        }
	}

void CFlightAnalysisDlg::OnEditCopy()
	{
	bool bAll=false;

	CString strCopy;
	LVCOLUMN cColumn;
	cColumn.mask=LVCF_TEXT;
	cColumn.pszText=(LPTSTR)malloc(sizeof(char) * 80);
	cColumn.cchTextMax=80;

	for(  int iCol = 0; iCol<NUM_COLUMNS; iCol++)
		{
		memset(cColumn.pszText,0,80);
		m_cListCtrl.GetColumn(iCol, &cColumn);  
		strCopy+=cColumn.pszText;
		strCopy+="\t";
		}
	strCopy+="\r\n";
	free(cColumn.pszText);

	if( bAll )
		{

		for(  int iRow = 0; iRow<m_pcFlight->GetNumPoints(); iRow++)
			{
			for(  int iCol = 0; iCol<NUM_COLUMNS; iCol++)
				{
				strCopy+=m_cListCtrl.GetItemText(iRow,iCol);
				strCopy+="\t";
				}
			strCopy+="\r\n";
			}
		}
	else
		{
		POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
			{
			//	TRACE(_T("No items were selected!\n"));
			}
		else
			{
			while (pos)
				{
				int iRow = m_cListCtrl.GetNextSelectedItem(pos);
				for(  int iCol = 0; iCol<NUM_COLUMNS; iCol++)
					{
					strCopy+=m_cListCtrl.GetItemText(iRow,iCol);
					strCopy+="\t";
					}
				strCopy+="\r\n";
				}
			}
		}
	size_t strLength=strCopy.GetLength();
    HGLOBAL hglobalText = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strLength);
	memcpy((char *)GlobalLock(hglobalText),strCopy,strLength);
    GlobalUnlock(hglobalText);
    
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,hglobalText);
    CloseClipboard();
	}


BOOL CFlightAnalysisDlg::PreTranslateMessage(MSG* pMsg) 
{
   if (GetKeyState(VK_CONTROL) & 0x8000 && pMsg->wParam == 'C')
	    {
        OnEditCopy();
		return true;
		}

   if (GetKeyState(VK_CONTROL) & 0x8000 && pMsg->wParam == 'A')
	    {
		m_cListCtrl.SetItemState(-1, LVIS_SELECTED, 0x000F);
		return true;
		}


	return CDialog::PreTranslateMessage(pMsg);
}

void CFlightAnalysisDlg::OnBnClickedPEV()
{
	int iPtr = m_vPEVPos[m_iPEVPtr++];
	if( m_iPEVPtr > (int)m_vPEVPos.size()-1 ) m_iPEVPtr=0;

	EnsureVisibleTop(  iPtr );
	m_cListCtrl.SetItemState(  iPtr, LVIS_SELECTED, LVIS_SELECTED);
}
