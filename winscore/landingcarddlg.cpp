//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// LandingCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "LandingCardDlg.h"
#include "LandoutDlg.h"
#include "TurnpointPickerDlg.h"
#include "global_prototypes.h"
#include "LandingLocationDlg.h"
#include "FlightDisplayDlg.h"
#include "GateDlg.h"
#include "ChangeTurnpointLocDlg.h"
#include "DontShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool fArmedCallback=true;
static bool fCardListModified=false;
static bool fCardModified=false;

/////////////////////////////////////////////////////////////////////////////
// CLandingCardDlg dialog


CLandingCardDlg::CLandingCardDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CLandingCardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLandingCardDlg)
	m_cContestNo = _T("");
	m_strDistanceText = _T("");
	m_bAirfield = FALSE;
	//}}AFX_DATA_INIT

    SetControlInfo(IDC_TURNPOINT_LIST,  RESIZE_BOTH);
    SetControlInfo(IDC_TURNPOINT_EDIT,  RESIZE_HOR);
    SetControlInfo(IDC_CHECKEDLIST,		RESIZE_BOTH);
    SetControlInfo(IDC_ACHEIVED_TEXT,	RESIZE_BOTH);
    SetControlInfo(IDOK,				ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,            ANCHORE_BOTTOM);
    SetControlInfo(IDRESET,             ANCHORE_BOTTOM);
    SetControlInfo(IDC_APPLY,			ANCHORE_BOTTOM);
    SetControlInfo(IDC_ADD_BUTTON,		ANCHORE_RIGHT );
	SetControlInfo(IDC_LANDING_LOCATION,ANCHORE_BOTTOM);
	SetControlInfo(IDC_LANDINGLOCATIONGROUP,ANCHORE_BOTTOM |RESIZE_HOR);
    SetControlInfo(IDC_DATE_COMBO,		RESIZE_HOR);
	m_pPreselect= NULL;
	m_pcFlight=NULL;
	m_iTurnpointSought=0;
	m_bPreselect=false;
	for( int i=0; i<MAXTASKTPS; i++) m_aiTurnpointFixes[i]=0;

}

void CLandingCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLandingCardDlg)
	DDX_Control(pDX, IDC_CHANGETURNPOINTBUTTON, m_cChangeTATLocationButton);
	DDX_Control(pDX, IDC_FLIGHTLOGGROUP, m_cFlightLogGroup);
	DDX_Control(pDX, IDC_VIEWLOG, m_cViewLogButton);
	DDX_Control(pDX, IDC_LANDINGLOCATIONGROUP, m_cLandingLocationText);
	DDX_Control(pDX, IDC_STARTGATEGROUP, m_cStartGateGroup);
	DDX_Control(pDX, IDRESET, m_cResetButton);
	DDX_Control(pDX, IDC_START_GATE_CHECK, m_cStartGateCheck);
	DDX_Control(pDX, IDC_STARTGATE, m_cStartGateButton);
	DDX_Control(pDX, IDC_LANDING_LOCATION, m_cLandingLocationButton);
	DDX_Control(pDX, IDC_CHECKEDLIST, m_cCheckedTurnpointListCtrl);
	DDX_Control(pDX, IDC_TURNPOINT_EDIT, m_cTurnpointEdit);
	DDX_Control(pDX, IDC_FINISHEDCHECK, m_cFinishedCheck);
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDC_NOTASK_TEXT, m_cStatusText);
	DDX_Control(pDX, IDC_CONTESTANT_COMBO, m_cContestantCombo);
	DDX_Control(pDX, IDC_TURNPOINT_LIST, m_cTurnpointList);
	DDX_Control(pDX, IDC_DISTANCE_TEXT, m_cDistanceText);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_cAddButton);
	DDX_CBString(pDX, IDC_CONTESTANT_COMBO, m_cContestNo);
	DDX_Text(pDX, IDC_DISTANCE_TEXT, m_strDistanceText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLandingCardDlg, CDialog)
	//{{AFX_MSG_MAP(CLandingCardDlg)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO, OnSelchangeDateCombo)
	ON_CBN_SELCHANGE(IDC_CONTESTANT_COMBO, OnSelchangeContestantCombo)
	ON_NOTIFY(NM_SETFOCUS, IDC_TURNPOINT_LIST, OnSetfocusTurnpointList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_TURNPOINT_LIST, OnKeydownTurnpointList)
	ON_BN_CLICKED(IDC_FINISHEDCHECK, OnFinishedcheck)
	ON_EN_UPDATE(IDC_TURNPOINT_EDIT, OnUpdateTurnpointEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHECKEDLIST, OnItemchangedCheckedlist)
	ON_NOTIFY(NM_CLICK, IDC_CHECKEDLIST, OnClickCheckedlist)
	ON_CBN_EDITUPDATE(IDC_CONTESTANT_COMBO, OnEditupdateContestantCombo)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_AIRFIELD, OnAirfield)
	ON_BN_CLICKED(IDC_LANDING_LOCATION, OnLandingLocation)
	ON_BN_CLICKED(IDC_STARTGATE, OnStartgate)
	ON_BN_CLICKED(IDC_START_GATE_CHECK, OnStartGateCheck)
	ON_BN_CLICKED(IDRESET, OnReset)
	ON_BN_CLICKED(IDC_VIEWLOG, OnViewlog)
	ON_BN_CLICKED(IDC_CHANGETURNPOINTBUTTON, OnChangeturnpointbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLandingCardDlg message handlers

void CLandingCardDlg::SetLandingCardList(CLandingCardList *pcLandingCardList)
{
m_pcLandingCardList=pcLandingCardList;
}

BOOL CLandingCardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Setup Combo Boxes
	m_pDoc->LoadDateComboBox(m_cDateCombo);
	m_pDoc->LoadContestantComboBox(m_cContestantCombo, TRUE);
	m_cTurnpointList.Initalize(  m_pDoc->m_eUnits, m_pDoc->GetUnitsText(), &m_pDoc->m_turnpointArray );
	m_cCheckedTurnpointListCtrl.Initalize(  m_pDoc->m_eUnits, m_pDoc->GetUnitsText(), &m_pDoc->m_turnpointArray, true, true, true  );
	m_nAcheivedTurnpoints=0;
	SetFormFromCard(NULL);


	if( m_pPreselect ) 
		{
		int iSel=m_cDateCombo.GetCurSel();
		ASSERT(iSel>=0);
		CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

		iSel=m_cContestantCombo.FindString( 0, m_pPreselect->m_cContestNo );
		m_cContestantCombo.SetCurSel( iSel );
		OnSelchangeContestantCombo();
		}
	else if( m_bPreselect )
		{
		int iSel=m_cContestantCombo.FindString( 0, m_strPreselectCID );
		m_cContestantCombo.SetCurSel( iSel );
		OnSelchangeContestantCombo();
		}


	fCardListModified=false;
	fCardModified=false;

	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	m_cSelectedDate=*(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	return TRUE; 
}

void CLandingCardDlg::OnAddButton() 
{
	CTurnpointPickerDlg dlg(m_pDoc->m_turnpointArray);	
	if( dlg.DoModal() ==IDOK )
		{
		if( dlg.m_pSelectedTurnpoint==NULL ) return;
		m_cTurnpointEdit.AddTurnpoint(dlg.m_pSelectedTurnpoint->m_iID);
		UpdateDistanceText();
		fCardModified=true;
		}
}


void CLandingCardDlg::OnOK() 
{
	if( fCardModified)
		OnApply();
	CDialog::OnOK();
}


void CLandingCardDlg::SetFormFromCard(CLandingCard* pcInputCard)
{
	CContestant*	pcContestant=GetContestant();
	CTask*			pcTask		=GetTask();
	CLandingCard*	pcLandingCard=NULL;

	int aiTurnpoints[MAXTASKTPS+1];
	m_cTurnpointEdit.Clear();


	if( pcInputCard==NULL )
		pcLandingCard=GetLandingCard();
	else
		pcLandingCard=pcInputCard;

	if( pcTask && (pcTask->m_eType==eAssigned || pcTask->m_eType==eTurnArea)  )
		{
		for( int i=0; i<pcTask->GetNumTurnpoints(); i++ )
			{
			aiTurnpoints[i]=pcTask->GetTurnpointID(i);
			CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(pcTask->GetTurnpointID(i)-1);
			if( pcTurnpoint )
				m_acTurnpointLocations[i]= *pcTurnpoint;
			m_aiTurnpointFixes[i]		 =0;
			}
		aiTurnpoints[pcTask->GetNumTurnpoints()]=pcTask->m_cFinishGate.GetPointID();
		}

	if( pcTask )
		{
		m_cStartGate = pcTask->GetActiveStartGate(GetContestant());
		m_cCheckedTurnpointListCtrl.m_bFixDisplay=pcTask->m_eType==eTurnArea;
		}

	// Check to see if there is a flight log
	if( pcContestant )
		m_pcFlight = m_pDoc->m_FlightList.Get(GetDate(), pcContestant->m_strContestNo);

	if( pcLandingCard==NULL )
		{		
		if( m_pcFlight )
			{  // No landing card and flight log present
			m_pcFlight->ReadFlight();
			m_pcFlight->m_pDoc=m_pDoc;
			m_pcFlight->Analyze(	GetTask(), 
									m_pDoc->m_turnpointArray,
									&m_pDoc->m_contestantList,
									m_pDoc->m_eUnits);
			m_pcFlight->FreePositionData();

			m_cFinishedCheck.SetCheck(m_pcFlight->IsTaskComplete());
			m_nAcheivedTurnpoints	=m_pcFlight->GetNumAcheivedTurnpoints();
			m_iLandoutTurnpoint		=0;//m_pcFlight->GetLandoutTurnpointID();
			m_iTurnpointSought		=m_pcFlight->GetSoughtTurnpoint();
			for( int i=0; i<m_nAcheivedTurnpoints; i++ )
				{
				m_cTurnpointEdit.AddTurnpoint( m_pcFlight->GetAcheivedTurnpoint(i));
				m_acTurnpointLocations[i]	=m_pcFlight->GetTurnAreaLocation(i);
				m_aiTurnpointFixes[i]		=m_pcFlight->GetTurnAreaFixID(i);
				}
			m_bAirfield=m_pcFlight->GetLandoutTurnpointID()>0;
	//		m_cLandoutLocation=m_pcFlight->GetFurthestProgressLocation();
			
			}
		else
			{ // No landing card and no flight log
			m_cFinishedCheck.SetCheck(0);
			m_nAcheivedTurnpoints	=0;
			m_iLandoutTurnpoint		=0;
			m_iTurnpointSought		=0;
			m_cStartGateCheck.SetCheck(false);
			m_bAirfield				=FALSE;
			m_pcFlight				=NULL;

			// Set the default landout coordinate to the task start point
			int iStart=m_cStartGate.GetPointID();
			CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(iStart-1);
			if( pcTurnpoint ) m_cLandoutLocation=*pcTurnpoint;
			}

		}
	else
		{ // Landing Card present 
		if( pcTask )
			{
			BOOL bUseBGate=false;

			m_cStartGateCheck.SetCheck(pcLandingCard->m_bLocalStartGate);
			m_cStartGate  = pcLandingCard->m_cStartGate;
			if( m_cStartGate.GetPointID()==0 )
				m_cStartGate = pcTask->GetActiveStartGate(GetContestant());

			m_cFinishedCheck.SetCheck(pcLandingCard->m_fCompleted);
			pcLandingCard->TurnpointArray(pcTask, aiTurnpoints);	

			m_nAcheivedTurnpoints=pcLandingCard->GetNumTurnpoints(pcTask);
			for( int i=0; i<m_nAcheivedTurnpoints; i++ )
				{
				// perform duplicate checks
				if( i==0 && aiTurnpoints[i]==m_cStartGate.GetPointID() ) continue;
				if( pcLandingCard->m_fCompleted &&
					i==m_nAcheivedTurnpoints-1	&& 
					aiTurnpoints[i]==pcTask->GetFinishGate().GetPointID() ) continue;

				if( /*pcTask->GetType()==ePost ||*/ pcTask->GetType()==eModifiedAssigned )
					m_cTurnpointEdit.AddTurnpoint( aiTurnpoints[i] );

				m_acTurnpointLocations[i]	=pcLandingCard->GetLocation(i);
				m_aiTurnpointFixes[i]		=pcLandingCard->GetFix(i);
				}
			m_cLandoutLocation=pcLandingCard->m_cLandoutLocation;
			m_iTurnpointSought=pcLandingCard->m_iTurnpointSought;
			m_iLandoutTurnpoint=pcLandingCard->m_iLandoutTurnpoint;
			m_bAirfield=pcLandingCard->m_bDesignatedAirfield;
			}
		}

	if ( m_iTurnpointSought==0 && pcTask )
		{
		m_iTurnpointSought=pcTask->GetFinishGate().GetPointID();
		}

	RefreshTaskList();
	m_cCheckedTurnpointListCtrl.UpdateDistance();
	MaskForm();
	UpdateData(FALSE);
	fCardModified=false;
}


void CLandingCardDlg::MaskForm()
{
	CTask* pcTask=GetTask();

	m_cFinishedCheck.EnableWindow(pcTask!=NULL);
	if( pcTask==NULL )
		{
		EnableAcheived( FALSE, NULL );
		EnableLanded(FALSE);
		m_cDistanceText.EnableWindow(	FALSE );
		m_cLandingLocationButton.EnableWindow(	FALSE );
		m_cStartGateButton.EnableWindow(	FALSE );
		m_cResetButton.EnableWindow(	FALSE );
		m_cStartGateGroup.SetWindowText("Start Gate");
		m_cLandingLocationText.SetWindowText("Landing Point");
		m_cLandingLocationText.EnableWindow( FALSE );
		m_cViewLogButton.EnableWindow( FALSE );
		m_cFlightLogGroup.EnableWindow( FALSE );
		m_cChangeTATLocationButton.EnableWindow( FALSE );
		}
	else
		{
		EnableAcheived( (pcTask->m_eType!=eAssigned || pcTask->m_eType==eTurnArea)||(!m_cFinishedCheck.GetCheck()), NULL );
		EnableLanded(!m_cFinishedCheck.GetCheck());
		m_cDistanceText.EnableWindow(	TRUE );
		m_cStartGateCheck.EnableWindow( TRUE );	
		m_cStartGateButton.EnableWindow(m_cStartGateCheck.GetCheck());	
		m_cResetButton.EnableWindow(true);//GetLandingCard()!=NULL);

		m_cLandingLocationText.SetWindowText(GetLandoutLocationText());

		CString strGate="Start Gate";
		int iStart=GetStartGate().GetPointID();
		CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(iStart-1);
		if( pcTurnpoint )
			{
			strGate+=":  ";
			strGate+=pcTurnpoint->GetNoNameText();
			}
		m_cStartGateGroup.SetWindowText(strGate);

		m_cViewLogButton.EnableWindow( m_pcFlight!=NULL );
		m_cFlightLogGroup.EnableWindow( m_pcFlight!=NULL );
		m_cChangeTATLocationButton.EnableWindow( m_pcFlight!=NULL && pcTask->m_eType==eTurnArea);
		}
	SetStatusText();

}

void CLandingCardDlg::EnableAcheived(BOOL fEnable, CTask* pcTask)
{
	m_cTurnpointEdit.EnableWindow(fEnable);
	m_cAddButton.EnableWindow(fEnable);
	m_cTurnpointList.EnableWindow(fEnable);
	m_cCheckedTurnpointListCtrl.EnableWindow(fEnable);
}

void CLandingCardDlg::EnableLanded(BOOL fEnable)
	{
	m_cLandingLocationButton.EnableWindow(	fEnable );
	m_cLandingLocationText.EnableWindow( fEnable );
	}


CLandingCard* CLandingCardDlg::GetLandingCard()
{
	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
	ASSERT(pcTime);	

	CContestant* pcContestant=GetContestant();
	if( pcContestant==NULL ) return NULL;

	return m_pcLandingCardList->Get( *pcTime,  pcContestant->m_strContestNo );
}

CTask* CLandingCardDlg::GetTask()
{
	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
	ASSERT( pcTime);

	CContestant* pcContestant=GetContestant();
	if( pcContestant==NULL ) return NULL;

	return m_pDoc->m_taskList.GetByDateClass( *pcTime, pcContestant->m_eClass);
}

void CLandingCardDlg::OnSelchangeDateCombo() 
{
	if( UpdateData()==0 ) return;
	SetFormFromCard(NULL);

	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	m_cSelectedDate=*(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
}


void CLandingCardDlg::OnSetfocusTurnpointList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CLandingCardDlg::UpdateDistanceText()
{
	/*
	double dDist=0.0;

	CLandingCard cLandingCard;
	SetLandingCardFromDlg(cLandingCard);
	dDist=cLandingCard.GetDistance( GetTask(),
									GetContestant(), 
									m_pDoc->m_turnpointArray,
									m_pDoc->m_eUnits);
	m_strDistanceText.Format("Distance Flown = %6.2lf %s ", dDist, m_pDoc->GetUnitsText() );

	UpdateData(FALSE);
	*/
}




void CLandingCardDlg::OnApply() 
{
//	if( !fCardModified ) return;
	CLandingCard cLandingCard;
	CTask *pcTask=GetTask();

	if( !SetLandingCardFromDlg(cLandingCard) ) return;

	//  Create the landing card only for finishers or landers
	m_pcLandingCardList->AddToList(&cLandingCard);
	fCardListModified=true;

	// Now re-analyze this flight
	if( m_pcFlight )//&& m_pcFlight->NotAnalyzed() )
			{
			m_pcFlight->ReadFlight();
			m_pcFlight->m_pDoc=m_pDoc;
			m_pcFlight->Analyze(	GetTask(), 
									m_pDoc->m_turnpointArray,
									&m_pDoc->m_contestantList,
									m_pDoc->m_eUnits);
			m_pcFlight->FreePositionData();
			}

	if( !m_pcFlight && 
		pcTask && pcTask->GetType()==eTurnArea &&
		m_cFinishedCheck.GetCheck() )
		{
		CString strWarn;
		strWarn = "Notice:  You indicated that this pilot finished a ";
		strWarn+= "turn area task, but no flight log was found.  Therefore ";
		strWarn+= "the nominal task distance will be used.\n";
		strWarn+= "If you import a flight log for this pilot, then the ";
		strWarn+= "landing card must be deleted or reset to get the proper distance.\n";

		CDontShowDlg	dlg( "TAT_LC", strWarn);
		dlg.DoModalConditional();
		}

	if( m_pcFlight && 
		pcTask && pcTask->GetType()==eTurnArea &&
		m_cFinishedCheck.GetCheck() )
		{
		CString strWarn;
		strWarn = "Notice: While scoring turn area tasks, landing cards are only needed ";
		strWarn+= "for pilots who either do not complete the task or if there is some problem ";
		strWarn+= "with the flight log analysis.\n";

		CDontShowDlg	dlg( "TAT_LC2", strWarn);
		dlg.DoModalConditional();
		}


	MaskForm();
	// Clear the contestant window
	m_cContestantCombo.SetFocus();
	fCardModified=false;
}



bool CLandingCardDlg::SetLandingCardFromDlg(CLandingCard &cLandingCard)
	{
	int iTp1=0;
	int iTp2=0;
	int iSel;

	if( UpdateData()==0 ) return false;

	iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
	cLandingCard.m_cDate= *pcTime;

	iSel=m_cContestantCombo.GetCurSel();
	ASSERT(iSel>=0);
	CContestant* pcContestant=(CContestant*)m_cContestantCombo.GetItemDataPtr(iSel);
	if( !pcContestant )
		{
		AfxMessageBox("Could not locate this contestant in the contestant list.");
		return false;
		}
	cLandingCard.m_cContestNo=pcContestant->m_strContestNo;

	cLandingCard.m_bLocalStartGate=m_cStartGateCheck.GetCheck()?true:false;
	cLandingCard.m_cStartGate=m_cStartGate;

	CTask* pcTask= GetTask();
	if( !pcTask )
		{
		//Warning no LandingCard
		AfxMessageBox( IDS_ERR_LNDCARD_TASK_NOT_FOUND, MB_OK  );
		return false;
		}

	cLandingCard.m_fCompleted=m_cFinishedCheck.GetCheck();
	if( /*pcTask->m_eType==ePost ||*/ pcTask->m_eType==eModifiedAssigned )
		{

		BOOL bUseBGate=false;
		if( pcTask->m_b2ndGateActive )
			{
			CContestant* pcContestant=GetContestant();
			if( pcContestant )
				bUseBGate = pcContestant->IsInBGroup();
			}

		cLandingCard.m_nTurnpoints=m_cTurnpointEdit.ParseTurnpoints( MAXTASKTPS, cLandingCard.m_aiTurnpoints);
		if( cLandingCard.m_nTurnpoints>MAXTASKTPS )
				{
				// Too Many turnpoints
				AfxMessageBox( IDS_ERR_LNDCRD_MAX_TURNPOINTS, MB_OK  );
				return false;
				}

		for( int i=0; i<cLandingCard.m_nTurnpoints+1; i++ )
			{
			if( i==0 )
				iTp1=m_cStartGate.GetPointID();
			else
				iTp1=cLandingCard.m_aiTurnpoints[i-1];

			if( i==cLandingCard.m_nTurnpoints)
				iTp2=pcTask->m_cFinishGate.GetPointID();
			else
				iTp2=cLandingCard.m_aiTurnpoints[i];

			if( m_pDoc->m_InvalidLegList.InvalidLeg( iTp1,
													 iTp2)  )
				{
				CString cTemp;
				cTemp.Format("Contestant claimed leg from %d to %d which was declared as an invalid leg.",
								iTp1,
								iTp2 );
				 AfxMessageBox( cTemp, MB_OK );
				 break;
				}
			}


		if( cLandingCard.Used2Turnpoints() )
			{
			if( AfxMessageBox( "Same turnpoint used twice without two others inbetween.", MB_OKCANCEL )!=IDOK ) return false;
			}
		}
	else
		{
		cLandingCard.m_nTurnpoints=m_nAcheivedTurnpoints;
		for( int i=0; i<m_nAcheivedTurnpoints; i++)
			cLandingCard.m_aiTurnpoints[i]=pcTask->GetTurnpointID(i);

		cLandingCard.m_bUseTPLocations = pcTask->GetType()==eTurnArea;
		if( cLandingCard.m_bUseTPLocations )
			{
			for( int i=0; i<m_nAcheivedTurnpoints; i++)
				{
				cLandingCard.SetFix(i, m_aiTurnpointFixes[i]);
				cLandingCard.SetLocation(i, m_acTurnpointLocations[i]);
				}
			}
		else
			{
			for( int i=0; i<m_nAcheivedTurnpoints; i++)
				{
				CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(pcTask->GetTurnpointID(i)-1);
				if( pcTurnpoint )
					cLandingCard.SetLocation(i, *pcTurnpoint);
				}

			}


		}	

	cLandingCard.m_iLandoutTurnpoint=m_iLandoutTurnpoint;
	cLandingCard.m_cLandoutLocation=m_cLandoutLocation;
	cLandingCard.m_bDesignatedAirfield=m_bAirfield;

	if( m_cFinishedCheck.GetCheck()==0 )
		cLandingCard.m_iTurnpointSought=m_iTurnpointSought;
	else
		cLandingCard.m_iTurnpointSought=0;

	return true;
	} 


BOOL CLandingCardDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{

	return CDialog::OnCommand(wParam, lParam);
}


void CLandingCardDlg::OnKeydownTurnpointList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	if( pLVKeyDow->wVKey==DELETE_KEY ) 
		{
		}	
	*pResult = 0;
}

void CLandingCardDlg::OnCancel() 
{
	if( fCardListModified==true )
		{
		CString cError="Save changes to landing cards?";
		int iAnswer=AfxMessageBox(cError, MB_YESNOCANCEL );
		if( iAnswer==IDCANCEL ) 
			return;
		else if( iAnswer==IDYES ) 
			{
			OnOK();
			return;
			}
		}
	CDialog::OnCancel();
}

void CLandingCardDlg::OnFinishedcheck() 
{
	// Reset the Turnpoint List Box
	RefreshTaskList();
	MaskForm();
	fCardModified=true;
}

void CLandingCardDlg::RefreshTaskList()
	{

	CTask* pcTask=GetTask();
	if(!pcTask ) 
		{
		SetListVisibility(eAssigned);
		m_cTurnpointList.ResetContent();
		m_cCheckedTurnpointListCtrl.ResetContent();
		return;
		}

	if( pcTask->m_eType==eAssigned || pcTask->m_eType==eTurnArea )
		{
		fArmedCallback=false;

		SetListVisibility(eAssigned);

		m_cCheckedTurnpointListCtrl.SetStartGate(GetStartGate());
		m_cCheckedTurnpointListCtrl.SetFinishGate(pcTask->GetFinishGate());
		m_cCheckedTurnpointListCtrl.ShowFinish( true );
		m_cCheckedTurnpointListCtrl.ResetContent();

		m_cCheckedTurnpointListCtrl.AddTurnpoint(GetStartGate().GetPointID(),TPT_START,GetStartGate().GetRadius());
		for( int i=0; i<pcTask->GetNumTurnpoints(); i++ )
			{
			m_cCheckedTurnpointListCtrl.AddTurnpoint(pcTask->GetTurnpointID(i),TPT_CONTROL,pcTask->GetTurnpointRadius(i));
			m_cCheckedTurnpointListCtrl.SetFix(i, m_aiTurnpointFixes[i], m_acTurnpointLocations[i]);
			}
		m_cCheckedTurnpointListCtrl.AddTurnpoint(pcTask->GetFinishGate().GetPointID(),TPT_FINISH,pcTask->GetFinishGate().GetRadius());

		// Now Set the checks
		if( m_cFinishedCheck.GetCheck() )
			m_nAcheivedTurnpoints=pcTask->GetNumTurnpoints();

		ListView_SetCheckState(m_cCheckedTurnpointListCtrl.m_hWnd, 0, 1);
		for( i=0; i<pcTask->GetNumTurnpoints(); i++ )
			{
			ListView_SetCheckState(m_cCheckedTurnpointListCtrl.m_hWnd, i+1, i<m_nAcheivedTurnpoints)
			}
		ListView_SetCheckState(	m_cCheckedTurnpointListCtrl.m_hWnd, 
								pcTask->GetNumTurnpoints()+1, 
								m_cFinishedCheck.GetCheck());

		if( m_iTurnpointSought==0 )
			{
			m_iTurnpointSought=pcTask->GetTurnpointID(m_nAcheivedTurnpoints);
			if( m_iTurnpointSought==0 )
				m_iTurnpointSought=pcTask->GetFinishGate().GetPointID();
			}

		m_cCheckedTurnpointListCtrl.UpdateDistance();
		fArmedCallback=true;
		}
	else
		{
		SetListVisibility(pcTask->m_eType);
		m_cTurnpointList.ShowFinish( m_cFinishedCheck.GetCheck()?(true):(false) );
		m_cTurnpointList.SetStartGate(GetStartGate());
		m_cTurnpointList.SetFinishGate(pcTask->GetFinishGate());

		int aiTurnpoints[MAXTASKTPS];
		int nParsed=m_cTurnpointEdit.ParseTurnpoints( MAXTASKTPS, aiTurnpoints);
		m_cTurnpointList.ResetContent();


		m_cTurnpointList.AddTurnpoint(GetStartGate().GetPointID(),TPT_START);
		for( int i=0; i<nParsed; i++ )
			{
			m_cTurnpointList.AddTurnpoint(aiTurnpoints[i]);
			}
		if( m_cFinishedCheck.GetCheck() )
			m_cTurnpointList.AddTurnpoint(pcTask->m_cFinishGate.GetPointID(),TPT_FINISH);
		}
	UpdateDistanceText();
	}

void CLandingCardDlg::OnUpdateTurnpointEdit() 
{
	fCardModified=true;
	RefreshTaskList();
}

void CLandingCardDlg::SetListVisibility(ETaskType eType)
{
	if( eType==eAssigned )
		{
		m_cCheckedTurnpointListCtrl.ShowWindow(SW_SHOW);
		m_cTurnpointEdit.ShowWindow(SW_HIDE);
		m_cAddButton.ShowWindow(SW_HIDE);
		m_cTurnpointList.ShowWindow(SW_HIDE);
		}
	else
		{
		m_cCheckedTurnpointListCtrl.ShowWindow(SW_HIDE);
		m_cTurnpointEdit.ShowWindow(SW_SHOW);
		m_cAddButton.ShowWindow(SW_SHOW);
		m_cTurnpointList.ShowWindow(SW_SHOW);
		}
}

void CLandingCardDlg::OnItemchangedCheckedlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( fArmedCallback )
		{
		CTask* pcTask=GetTask();
		if(pcTask && (pcTask->m_eType==eAssigned || pcTask->m_eType==eTurnArea) )
			{
			m_nAcheivedTurnpoints=m_cCheckedTurnpointListCtrl.GetNumTurnpoints();
			m_iTurnpointSought=pcTask->GetTurnpointID(m_nAcheivedTurnpoints);
			RefreshTaskList();
			fCardModified=true;
			MaskForm();
			}	
		}
	*pResult = 0;
}

void CLandingCardDlg::OnClickCheckedlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CLandingCardDlg::OnEditupdateContestantCombo() 
{
	return;	
}


void CLandingCardDlg::OnSelchangeContestantCombo() 
{	
	if( UpdateData()==0 ) return;
	SetFormFromCard(NULL);
	return;
}


void CLandingCardDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CLandingCardDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);	
}

void CLandingCardDlg::OnAirfield() 
{
	fCardModified=true;
}


CContestant* CLandingCardDlg::GetContestant()
	{
	int iSel=m_cContestantCombo.GetCurSel();
	if( iSel<0 ) return NULL;

	CContestant* pcContestant=(CContestant*)m_cContestantCombo.GetItemDataPtr(iSel);
	if( pcContestant==NULL ) return NULL;

	return pcContestant;
	}

void CLandingCardDlg::OnLandingLocation() 
{/*
	int nTurnpoints=0, aiTurnpoints[MAXTASKTPS];
	CTask *pcTask=GetTask();

	if(pcTask && (pcTask->m_eType==eAssigned || pcTask->m_eType==eTurnArea))
		{
		nTurnpoints=m_cCheckedTurnpointListCtrl.GetNumTurnpoints();
		for( int i=0; i<nTurnpoints; i++)
			aiTurnpoints[i]=pcTask->GetTurnpointID(i);
		}
	else
		{
		nTurnpoints=m_cTurnpointEdit.ParseTurnpoints(MAXTASKTPS, aiTurnpoints);
		}

	CLandingLocationDlg dlg(	m_pDoc, 
								pcTask, 
								nTurnpoints, 
								aiTurnpoints,
								m_iTurnpointSought,
								m_cLandoutLocation,
								m_iLandoutTurnpoint,
								m_bAirfield?true:false);
	if( dlg.DoModal()==IDOK )
		{
		m_iTurnpointSought=dlg.m_iTurnpointSought;
		m_cLandoutLocation=dlg.m_cLandoutLocation;
		m_iLandoutTurnpoint=dlg.m_iLandoutTurnpoint;
		m_bAirfield=dlg.m_bAirfieldBonus;
		fCardModified=true;
		UpdateDistanceText();
		MaskForm();
		}*/
}

void CLandingCardDlg::OnStartgate() 
{
	CGateDlg dlg( m_pDoc, CGateDlg::eStart, m_cStartGate);
	if( dlg.DoModal()==IDOK )
		{
		m_cStartGate=dlg.m_cGate;
		RefreshTaskList();
		UpdateDistanceText();
		MaskForm();
		}
}


void CLandingCardDlg::OnStartGateCheck() 
	{
	m_cStartGateButton.EnableWindow(m_cStartGateCheck.GetCheck());	
	RefreshTaskList();
	UpdateDistanceText();
	MaskForm();
	}

CGate& CLandingCardDlg::GetStartGate()
	{
	if( m_cStartGateCheck.GetCheck() )
		{
		return m_cStartGate;
		}
	else
		{
		CTask *pcTask=GetTask();
		CContestant *pcContestant=GetContestant();
		return pcTask->GetActiveStartGate(pcContestant);
		}
	}

CTime& CLandingCardDlg::GetDate()
	{
	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
	return *pcTime;
	}

void CLandingCardDlg::SetStatusText()
{
	if( GetTask()==NULL )
		{
		m_cStatusText.SetWindowText("Status: No task has been defined for this contestant on this day.");
		return;
		}

	CString strStatus="Status: ";
	CLandingCard *pcLandingCard=GetLandingCard();

	if( pcLandingCard )
		strStatus+="Card Saved";
	else
		strStatus+="Not Saved";

	if( m_pcFlight )
		strStatus+=", Flight log found :"+m_pcFlight->m_strFileName;
	else
		strStatus+=", No flight log found";

	m_cStatusText.SetWindowText(strStatus);

}

void CLandingCardDlg::OnReset() 
	{
	m_pcLandingCardList->RemoveByPointer( GetLandingCard() );

	SetFormFromCard(NULL);

	fCardModified=FALSE;
	MaskForm();
	}

CString CLandingCardDlg::GetLandoutLocationText()
	{
	if( m_cFinishedCheck.GetCheck() )
		return "Landing Point";

	CString cText="Landing Point: ";
	if( m_iLandoutTurnpoint<=0 )
		{
		cText+=m_cLandoutLocation.GetFormattedInfo();
		}
	else
		{
		cText="Landed At: ";
		CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(m_iLandoutTurnpoint-1);
		if( pcTurnpoint )
			cText+=pcTurnpoint->GetNoNameText();
		else
			cText+="???";
		}
	if( m_cFinishedCheck.GetCheck() )
		return cText;

	CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(m_iTurnpointSought-1);
	if( pcTurnpoint )
		{
		cText+=", while seeking: ";
		cText+=pcTurnpoint->GetNoNameText();
		}

	return cText;
	}

void CLandingCardDlg::OnViewlog() 
{
	if( !m_pcFlight ) return;

	CLandingCard cLandingCard;
	SetLandingCardFromDlg(cLandingCard) ;

	if( !m_pcFlight->ReadFlight() )
		{
		AfxMessageBox("IGC file for this pilot could not be found.\n Cannot display without it.");
		return;
		}

	m_pcFlight->m_pDoc=m_pDoc;
	m_pcFlight->Analyze(	GetTask(), 
							m_pDoc->m_turnpointArray,
							&m_pDoc->m_contestantList,
							m_pDoc->m_eUnits);

	CFlightDisplayDlg dlg;
	dlg.m_pcFlight=m_pcFlight;
	dlg.m_pcLandingCard=&cLandingCard;
	dlg.m_pcTask=GetTask();
	dlg.m_pDoc=m_pDoc;
	dlg.DoModal();

	m_pcFlight->FreePositionData();	
}

void CLandingCardDlg::OnChangeturnpointbutton() 
	{
	CLandingCard cLandingCard;
	SetLandingCardFromDlg(cLandingCard);

	m_pcFlight->ReadFlight();
	m_pcFlight->m_pDoc=m_pDoc;
	m_pcFlight->Analyze(	GetTask(), 
							m_pDoc->m_turnpointArray,
							&m_pDoc->m_contestantList,
							m_pDoc->m_eUnits);


///	CChangeTurnpointLocDlg	dlg(m_pDoc,
//								GetContestant(),
//								GetTask(),
//								m_pcFlight,
//								cLandingCard);
//	if( dlg.DoModal()==IDOK )
//		{
//		SetFormFromCard(&cLandingCard);
//		}


	m_pcFlight->FreePositionData();
	}
