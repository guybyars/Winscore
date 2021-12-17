//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// EditFlightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "flight.h"
#include "turnpointcombo.h"
#include "EditFlightDlg.h"
#include "global_prototypes.h"
#include "LandoutDlg.h"
#include "LandingLocationDlg.h"
#include "ChangeTurnpointLocDlg.h"
#include "AlternateStartDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditFlightDlg dialog


CEditFlightDlg::CEditFlightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFlightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditFlightDlg)
	m_strEditTurnpoints = _T("");
	m_cTimePickerLanding = 0;
	m_cTimePickerFinish = 0;
	m_cTimePickerRoll = 0;
	m_cTimePickerStart = 0;
	m_bAirfieldBonus = FALSE;
	m_dDistance = 0.0;
	m_strScoredLandingPoint = _T("");
	//}}AFX_DATA_INIT
	m_pcTask=NULL;
	m_bDirty=false;
}


void CEditFlightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFlightDlg)
	DDX_Control(pDX, IDC_SAFETY_CHECK, m_cSafetyCheck);
	DDX_Control(pDX, IDC_ALT_START, m_cAltStartButton);
	DDX_Control(pDX, IDC_CHANGLANDOUTBUTTON, m_cChangeScoredLandingPointButton);
	DDX_Control(pDX, IDC_SCOREDLPCHECK, m_cScoredLandingPointCheck);
	DDX_Control(pDX, IDC_TURNPOINTCHECK, m_cTurnpointCheck);
	DDX_Control(pDX, IDC_AIRFIELD, m_cAirfieldBonus);
	DDX_Control(pDX, IDC_ATTEMPTING_TEXT, m_cAttemptingText);
	DDX_Control(pDX, IDC_LANDOUTGROUP, m_cLandoutGroup);
	DDX_Control(pDX, IDC_EDITDISTANCE, m_cEditDistance);
	DDX_Control(pDX, IDC_DISTANCECHECK, m_cDistanceCheck);
	DDX_Control(pDX, IDC_ROLLCHECK, m_cRollTimeCheck);
	DDX_Control(pDX, IDC_STARTCHECK, m_cStartTimeCheck);
	DDX_Control(pDX, IDC_LANDEDCHECK, m_cLandingTimeCheck);
	DDX_Control(pDX, IDC_FINISHCHECK, m_cFinishTimeCheck);
	DDX_Control(pDX, IDC_ATTEMPTING_COMBO, m_cAttemptingCombo);
	DDX_Control(pDX, IDC_START_COMBO, m_cStartCombo);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FINISH, m_cTimePickerFinishCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_cTimePickerStartCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ROLL, m_cTimePickerRollCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_LANDING, m_cTimePickerLandingCtrl);
	DDX_Control(pDX, IDC_EDIT_TURNPOINTS, m_cEditTurnpoints);
	DDX_Control(pDX, IDC_COMPLETED_TASK, m_cCompletedCheck);
	DDX_Text(pDX, IDC_EDIT_TURNPOINTS, m_strEditTurnpoints);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LANDING, m_cTimePickerLanding);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_FINISH, m_cTimePickerFinish);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ROLL, m_cTimePickerRoll);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START, m_cTimePickerStart);
	DDX_Check(pDX, IDC_AIRFIELD, m_bAirfieldBonus);
	DDX_Text(pDX, IDC_EDITDISTANCE, m_dDistance);
	DDV_MinMaxDouble(pDX, m_dDistance, 0., 3000.);
	DDX_Text(pDX, IDC_LANDOUTEDIT, m_strScoredLandingPoint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditFlightDlg, CDialog)
	//{{AFX_MSG_MAP(CEditFlightDlg)
	ON_BN_CLICKED(IDC_COMPLETED_TASK, OnCompletedCheck)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_FINISH, OnDatetimechangeDatetimepickerFinish)
	ON_BN_CLICKED(IDC_LANDING_LOCATION, OnLandingLocation)
	ON_BN_CLICKED(IDC_LANDEDCHECK, OnLandedcheck)
	ON_BN_CLICKED(IDC_FINISHCHECK, OnFinishcheck)
	ON_BN_CLICKED(IDC_STARTCHECK, OnStartcheck)
	ON_BN_CLICKED(IDC_ROLLCHECK, OnRollcheck)
	ON_BN_CLICKED(IDC_DISTANCECHECK, OnDistancecheck)
	ON_BN_CLICKED(IDC_UPDATE_DISTANCE, OnRecalculate)
	ON_BN_CLICKED(IDC_TURNPOINTCHECK, OnTurnpointcheck)
	ON_BN_CLICKED(IDC_CHANGLANDOUTBUTTON, OnChanglandoutbutton)
	ON_BN_CLICKED(IDC_SCOREDLPCHECK, OnScoredlpcheck)
	ON_BN_CLICKED(IDC_CHANGEFIXES, OnChangefixes)
	ON_EN_CHANGE(IDC_EDIT_TURNPOINTS, OnChangeEditTurnpoints)
	ON_CBN_EDITCHANGE(IDC_ATTEMPTING_COMBO, OnEditchangeAttemptingCombo)
	ON_CBN_SELCHANGE(IDC_ATTEMPTING_COMBO, OnSelchangeAttemptingCombo)
	ON_BN_CLICKED(IDC_AIRFIELD, OnAirfield)
	ON_BN_CLICKED(IDC_ALT_START, OnAltStart)
	ON_BN_CLICKED(IDC_SAFETY_CHECK, OnSafetyCheck)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_START, OnDatetimechangeDatetimepickerStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditFlightDlg message handlers

void CEditFlightDlg::OnOK() 
{
	CWaitCursor cWait;
	UpdateData(true);
	if( m_bDirty && !m_cDistanceCheck.GetCheck())
		{
		OnRecalculate();
		}
	else
		{
	    UpdateFlightFromDlg();
		}

	m_pcFlight->SetLock(true);

	CDialog::OnOK();
}

void CEditFlightDlg::UpdateFlightFromDlg() 
	{

	CTurnpoint* pcStart=m_cStartCombo.GetCurSel();
	if( pcStart )
		{
		m_pcFlight->SetStartGateID(pcStart->GetID());
		}

	if( m_cCompletedCheck.GetCheck() )
		{
		// Completed
	
		m_pcFlight->SetFinishValidity(true);

		if( m_cFinishTimeCheck.GetCheck() )
			{
			m_pcFlight->SetOption(FLT_FINISHTIMELOCKED);
			m_pcFlight->SetFinishTime(m_cTimePickerFinish);
			}	

		m_pcFlight->SetTurnpointsAchevied(true);
		m_pcFlight->SetNumAcheivedTurnpoints(m_pcFlight->GetNumTaskTurnpoints());
		m_pcFlight->SetStatus(eCompletedTask);
		
		for( int i=0; i<m_pcFlight->GetNumTaskTurnpoints(); i++)
			{
			m_pcFlight->SetAcheviedTurnpoint(i, m_pcFlight->GetTaskPointID(i));
			}
		m_pcFlight->AirfieldBonus(false);

		if( m_cSafetyCheck.GetCheck() )
			m_pcFlight->SetOption(FLT_SAFETYFINISH);
		else
			m_pcFlight->ClearOption(FLT_SAFETYFINISH);
		}
	else
		{
		// not completed
		m_pcFlight->SetFinishValidity(false);

		m_pcFlight->SetNumAcheivedTurnpoints(m_nAcheivedPts);
		m_pcFlight->SetStatus(	eIncompleteTask);
		m_pcFlight->ClearOption(FLT_AIRFIELDBONUSLOCKED);
		m_pcFlight->AirfieldBonus(m_cAirfieldBonus.GetCheck()?true:false);
		m_pcFlight->SetOption(FLT_AIRFIELDBONUSLOCKED);
		m_pcFlight->ClearOption(FLT_SAFETYFINISH);

		if( m_cScoredLandingPointCheck.GetCheck() ) 
			m_pcFlight->SetOption(FLT_SLANDINGPOINTLOCKED);
		else
			m_pcFlight->ClearOption(FLT_SLANDINGPOINTLOCKED);
		}

    if( m_pcTask->m_eType==eModifiedAssigned )
		{
		CTurnpoint *pTurnpointSought=m_cAttemptingCombo.GetCurSel();
		if( pTurnpointSought ) 
			m_pcFlight->SetSoughtTurnpoint(pTurnpointSought->GetID());
		}
	else
		m_pcFlight->SetSoughtTurnpoint(-1);

	if( m_cRollTimeCheck.GetCheck() )
		{
		m_pcFlight->SetRollTime(m_cTimePickerRoll);
		m_pcFlight->SetOption(FLT_ROLLTIMELOCKED);
		}
	if( m_cLandingTimeCheck.GetCheck() )
		{
		m_pcFlight->SetLandingTime(m_cTimePickerLanding);
		m_pcFlight->SetOption(FLT_LANDINGTIMELOCKED);
		}

	if( m_cDistanceCheck.GetCheck() )
		{
		m_pcFlight->SetDistance(ConvertDistance(m_dDistance,m_pDoc->m_eUnits,SYSTEMUNITS));
		m_pcFlight->SetOption(FLT_DISTANCELOCKED);
		}

	if( m_cStartTimeCheck.GetCheck() )
		{
		m_pcFlight->SetOption(FLT_STARTTIMELOCKED);
		if( m_pcTask && m_pcTask->m_cStartGate.IsFirstLegAtExit() )
			{
	        if( m_pcFlight->GetStartTime()!=m_cTimePickerStart)
				{
				m_pcFlight->SetStartTime(m_cTimePickerStart);
				if( !m_cDistanceCheck.GetCheck() )
					{
					if( !m_pcFlight->ReadFlight() )
						{
						AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot update distance without it."));
						return;
						}
					m_pcFlight->SetLock(true);
					// Need to do a full analysis here.  It is possible for someone to go out and acheive a Turn area
					// turnpoint, then come back for a later start.
					m_pcFlight->Analyze(	m_pcTask, 
											m_pDoc->m_turnpointArray,
											&m_pDoc->m_contestantList,
											m_pDoc->m_eUnits);
					/*
					m_pcFlight->FindStartFix();
					m_pcFlight->SetDistance(m_pcFlight->ComputeDistance(m_pcTask, m_pDoc->m_turnpointArray));
					*/
					}
				}
			else
				m_pcFlight->SetStartTime(m_cTimePickerStart);
			}
		else
			{
			m_pcFlight->SetStartTime(m_cTimePickerStart);
			}
		}	
	}

void CEditFlightDlg::OnCompletedCheck() 
	{
	UpdateData();
	MaskForm();
	m_bDirty=true;
	}

BOOL CEditFlightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cTitle=_T("Edit Flight for: ")+m_pcFlight->m_strCID;
	cTitle+=_T(" - ")+m_pcFlight->m_strPilot;
	SetWindowText(cTitle);
	
	CContestant *pcContestant=m_pDoc->m_contestantList.GetByContestNo(m_pcFlight->m_strCID);
	ASSERT(pcContestant!=NULL);

	m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cStartCombo, TPT_START, m_pcFlight->GetStartGateID());

	int iTurnpointSought=m_pcFlight->GetSoughtTurnpoint();
	m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cAttemptingCombo, TPT_CONTROL | TPT_FINISH | TPT_START, iTurnpointSought);

	m_strEditTurnpoints=m_pcFlight->GetTaskPointText(m_pcFlight->GetNumAcheivedTurnpoints());
	m_nAcheivedPts=m_pcFlight->GetNumAcheivedTurnpoints();

	m_cCompletedCheck.SetCheck( m_pcFlight->IsTaskComplete() );

	m_cTimePickerLandingCtrl.SetFormat	(_T("HH : mm : ss") );
	m_cTimePickerRollCtrl.SetFormat		(_T("HH : mm : ss") );
	m_cTimePickerStartCtrl.SetFormat	(_T("HH : mm : ss") );
	m_cTimePickerFinishCtrl.SetFormat	(_T("HH : mm : ss") );

	m_cScoredLandingPointCheck.SetCheck(m_pcFlight->CheckOption(FLT_SLANDINGPOINTLOCKED));

	m_cDistanceCheck.SetCheck(m_pcFlight->CheckOption(FLT_DISTANCELOCKED));

	UpdateDlgFromFlight();

	m_cRollTimeCheck.SetCheck(m_pcFlight->CheckOption(FLT_ROLLTIMELOCKED));
	m_cStartTimeCheck.SetCheck(m_pcFlight->CheckOption(FLT_STARTTIMELOCKED));
	m_cFinishTimeCheck.SetCheck(m_pcFlight->CheckOption(FLT_FINISHTIMELOCKED));
	m_cLandingTimeCheck.SetCheck(m_pcFlight->CheckOption(FLT_LANDINGTIMELOCKED));
	m_cTurnpointCheck.SetCheck(m_pcFlight->CheckOption(FLT_TURNPOINTSLOCKED));
	m_cSafetyCheck.SetCheck(m_pcFlight->CheckOption(FLT_SAFETYFINISH));

	MaskForm();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditFlightDlg::MaskForm()
	{

	m_cTimePickerRollCtrl.EnableWindow(m_cRollTimeCheck.GetCheck());
	m_cTimePickerStartCtrl.EnableWindow(m_cStartTimeCheck.GetCheck());
	m_cTimePickerLandingCtrl.EnableWindow(m_cLandingTimeCheck.GetCheck());
	m_cEditDistance.SetReadOnly(!m_cDistanceCheck.GetCheck());

	if( m_pcTask && m_pcTask->m_eType!=eModifiedAssigned )
		{
		m_cEditTurnpoints.SetReadOnly();
		m_cTurnpointCheck.EnableWindow(false);
		m_cAttemptingCombo.EnableWindow(false);
		}
	else
		{
		m_cEditTurnpoints.SetReadOnly(!m_cTurnpointCheck.GetCheck());
	    m_cAttemptingCombo.EnableWindow(!m_cCompletedCheck.GetCheck());
		}


	if( !m_cCompletedCheck.GetCheck() )
		{
		m_cTimePickerFinishCtrl.EnableWindow(false);
		m_cFinishTimeCheck.EnableWindow(FALSE);
		}
	else
		{
		m_cTimePickerFinishCtrl.EnableWindow();
		m_cTimePickerFinishCtrl.EnableWindow(m_cFinishTimeCheck.GetCheck());
		m_cFinishTimeCheck.EnableWindow();
		}

	m_cSafetyCheck.EnableWindow(m_cCompletedCheck.GetCheck());
	m_cAirfieldBonus.EnableWindow(!m_cCompletedCheck.GetCheck());
	m_cAttemptingText.EnableWindow(!m_cCompletedCheck.GetCheck());
	m_cLandoutGroup.EnableWindow(!m_cCompletedCheck.GetCheck());
	m_cChangeScoredLandingPointButton.EnableWindow(!m_cCompletedCheck.GetCheck() && m_cScoredLandingPointCheck.GetCheck() );
	m_cScoredLandingPointCheck.EnableWindow(!m_cCompletedCheck.GetCheck());

	}


void CEditFlightDlg::OnDeltaposTpSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_nAcheivedPts=pNMUpDown->iPos;
	m_strEditTurnpoints=m_pcFlight->GetTaskPointText( pNMUpDown->iPos );
	UpdateData(FALSE);

	*pResult = 0;
}



void CEditFlightDlg::OnDatetimechangeDatetimepickerFinish(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}


void CEditFlightDlg::OnLandingLocation() 
{/*
	CLandingLocationDlg dlg(	m_pDoc, 
								m_pcFlight->GetLandingLocation(),
								m_pcFlight->GetFurthestProgressLocation(),
								m_pcFlight->GetLandoutTurnpointID(),
								m_pcFlight->UseFurthestProgress());
	if( dlg.DoModal()==IDOK )
		{
		m_pcFlight->SetLandoutTurnpointID(dlg.m_iLandoutTurnpoint);
		m_pcFlight->SetLandoutTurnpointName(dlg.m_strLandoutTurnpointName);
		m_pcFlight->SetLandingLocation(dlg.m_cLandoutLocation);
		m_pcFlight->SetFurthestProgress(dlg.m_bUseFurthestProgress);
		MaskForm();
		}	*/
}

void CEditFlightDlg::OnLandedcheck() 
{
MaskForm();	
m_bDirty=true;
}

void CEditFlightDlg::OnFinishcheck() 
{
MaskForm();	
m_bDirty=true;
}

void CEditFlightDlg::OnStartcheck() 
{
MaskForm();	
m_bDirty=true;
}

void CEditFlightDlg::OnRollcheck() 
{
MaskForm();	
m_bDirty=true;
}

void CEditFlightDlg::OnDistancecheck() 
{
MaskForm();	
}

void CEditFlightDlg::OnRecalculate() 
{
	UpdateData();

	if( m_cDistanceCheck.GetCheck() ) 
		{
		AfxMessageBox(_T("No need to recalculate if you have manually specified a distance."));
		return;
		}

	UpdateFlightFromDlg();

	if( !m_pcFlight->ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot display without it."));
		return;
		}

	m_pcFlight->m_pDoc=m_pDoc;


	if (m_cTurnpointCheck.GetCheck() )
		{
		m_pcFlight->SetOption(FLT_TURNPOINTSLOCKED);

		if( m_pcTask && m_pcTask->m_eType==eModifiedAssigned )
			{
			m_pcFlight->SetNumAcheivedTurnpoints(0);
			m_pcFlight->m_nTaskPoints=0;
			int nNum=0;
			int iaTpts[MAXTASKTPS];
			nNum=m_cEditTurnpoints.ParseTurnpoints( MAXTASKTPS, iaTpts );
			for( int i=0; i<nNum; i++)
				{
				CTurnpoint *pcTurnpoint=m_pDoc->m_turnpointArray[iaTpts[i]-1];
				if( pcTurnpoint ) m_pcFlight->SetTaskPoint( pcTurnpoint );

				}
			m_nAcheivedPts=nNum;

			// Put the sought turnoint as the final task point, assuming it is not the last one already
			if( !m_cCompletedCheck.GetCheck()  )
				{
				CTurnpoint *pTurnpointSought=m_cAttemptingCombo.GetCurSel();
				if( pTurnpointSought && pTurnpointSought->GetID()!=m_pcFlight->GetTaskPointID(m_pcFlight->m_nTaskPoints) ) 
					m_pcFlight->SetTaskPoint( pTurnpointSought );
				}
			}
		}
	else
		{
		m_pcFlight->m_nTaskPoints=0;
		m_pcFlight->SetNumAcheivedTurnpoints(0);
		m_pcFlight->ClearOption(FLT_TURNPOINTSLOCKED);
		}

	m_pcFlight->SetLock(true);
	m_pcFlight->Analyze(	m_pcTask, 
							m_pDoc->m_turnpointArray,
							&m_pDoc->m_contestantList,
							m_pDoc->m_eUnits);


	UpdateDlgFromFlight();
	m_pcFlight->FreePositionData();
	m_bDirty=false;

	UpdateData(false);
}


void CEditFlightDlg::Recalculate()
{

}




void CEditFlightDlg::OnTurnpointcheck() 
{
	MaskForm();	
	m_bDirty=true;
}


void CEditFlightDlg::UpdateDlgFromFlight()
	{
	m_dDistance=ConvertDistance(m_pcFlight->GetDistance(),SYSTEMUNITS, m_pDoc->m_eUnits);

	m_strEditTurnpoints=m_pcFlight->GetTaskPointText(m_pcFlight->GetNumAcheivedTurnpoints());

	m_cTimePickerLanding=m_pcFlight->GetLandingTime();
	if( m_pcFlight->IsFinishTimeValid() )
		m_cTimePickerFinish	=m_pcFlight->GetFinishTime();
	else
		m_cTimePickerFinish	=m_pcFlight->GetLandingTime();

	if( m_cStartTimeCheck.GetCheck() )
		{
		m_cTimePickerStart	=m_pcFlight->GetStartTime();
		}
	else
		{
	    if( m_pcFlight->IsStartTimeValid() )
		    m_cTimePickerStart	=m_pcFlight->GetStartTime();
	    else
		    m_cTimePickerStart	=m_pcFlight->GetRollTime();
		}

	m_cTimePickerRoll	=m_pcFlight->GetRollTime();

	m_bAirfieldBonus=m_pcFlight->IsAirfieldBonus();

	m_strScoredLandingPoint=m_pcFlight->GetScoredLandingPointLocation().GetFormattedInfo();

	int iTurnpointSought=m_pcFlight->GetSoughtTurnpoint();
	m_cAttemptingCombo.SetCurSel(iTurnpointSought);

	}


void CEditFlightDlg::OnChanglandoutbutton() 
{
	CLandoutDlg	dlg;
	dlg.m_cLocation=m_pcFlight->GetScoredLandingPointLocation();
	dlg.m_pDoc=m_pDoc;

	if (dlg.DoModal() == IDOK)
		{
		m_pcFlight->SetScoredLandingPointLocation(dlg.m_cLocation);
		m_strScoredLandingPoint =dlg.m_cLocation.GetFormattedInfo();
		UpdateData(false);
		m_bDirty=true;
		}		
}

void CEditFlightDlg::OnScoredlpcheck() 
{
MaskForm();	
}

void CEditFlightDlg::OnChangefixes() 
{
	
	m_pcFlight->m_pDoc=m_pDoc;
	CContestant *pcContestant=m_pDoc->m_contestantList.GetByContestNo(m_pcFlight->m_strCID);

	if( !m_pcFlight->ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot display without it."));
		return;
		}

	// Save the fix ID's and Locations in case he cancels.
	int aiFixIDs[MAXTASKTPS];
	CLocation acTATLocs[MAXTASKTPS];
	for( int i=0; i<MAXTASKTPS; i++ )
		{
		aiFixIDs[i]=m_pcFlight->GetTurnAreaFixID(i);
		acTATLocs[i]=m_pcFlight->GetTurnAreaLocation(i);
		}

	CChangeTurnpointLocDlg	dlg(m_pDoc,
								pcContestant,
								m_pcTask,
								m_pcFlight);
	if( dlg.DoModal()==IDOK )
		{
		m_dDistance=ConvertDistance(m_pcFlight->GetDistance(),SYSTEMUNITS, m_pDoc->m_eUnits);
        m_pcFlight->SetOption(FLT_OPTTURNPOINTSLOCKED);
		m_bDirty=false;
		}
	else
		{
		for( int i=0; i<MAXTASKTPS; i++ )
			{
			m_pcFlight->SetTurnAreaFixID(i,aiFixIDs[i]);
			m_pcFlight->SetTurnAreaLocation(i,acTATLocs[i]);
			}
		m_pcFlight->SetDistance(m_pcFlight->ComputeDistance(m_pcTask, m_pDoc->m_turnpointArray));
		}
	UpdateData(false);

	m_pcFlight->FreePositionData();	

}


void CEditFlightDlg::OnChangeEditTurnpoints() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	m_bDirty=true;	
}

void CEditFlightDlg::OnEditchangeAttemptingCombo() 
{
	// TODO: Add your control notification handler code here
	m_bDirty=true;	
}

void CEditFlightDlg::OnSelchangeAttemptingCombo() 
{
	// TODO: Add your control notification handler code here
	m_bDirty=true;	
}

void CEditFlightDlg::OnAirfield() 
{
	// TODO: Add your control notification handler code here
	m_bDirty=true;		
}

void CEditFlightDlg::OnAltStart() 
{
	bool bAlreadyBest=false;
	CWaitCursor cWait;

	if( !m_pcFlight->ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot proceed without it."));
		return;
		}

	m_pcFlight->Analyze(	m_pcTask, 
							m_pDoc->m_turnpointArray,
							&m_pDoc->m_contestantList,
							m_pDoc->m_eUnits);

	CAlternateStartDlg dlg;
	dlg.m_pDoc=m_pDoc;

	CWSClass &cClass	=GetClass(m_pcTask->m_eClass);
	dlg.m_bHandicapped=cClass.IsHandicapped();

	dlg.m_nStarts = m_pDoc->GetMultipleStartPoints(	m_pcFlight, 
													m_pcTask,
													dlg.m_iBestScorePtr,
													bAlreadyBest,
													dlg.m_nMaxRet, 
													dlg.m_caStartTimes, 
													dlg.m_caTOCs, 
													dlg.m_daSpeeds, 
													dlg.m_daDistances, 
													dlg.m_iaPointDiff, 
													dlg.m_iaPenalties, 
													dlg.m_iaScore  );

	if( bAlreadyBest) 
		{
		if( AfxMessageBox( _T("This flight is already using the best scoring start.  Continue anyway?"), MB_YESNOCANCEL  )!=IDYES ) return;
		}

	if( dlg.DoModal()== IDOK && dlg.m_cStartTime>0)
		{
		m_cTimePickerStart	=dlg.m_cStartTime;
		m_cTimePickerStartCtrl.EnableWindow(true);
		m_cStartTimeCheck.SetCheck(1);
		UpdateData(false);
		}

	m_pcFlight->FreePositionData();
}

void CEditFlightDlg::OnSafetyCheck() 
	{
	m_bDirty=true;
	}

void CEditFlightDlg::OnDatetimechangeDatetimepickerStart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
