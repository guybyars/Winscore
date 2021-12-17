//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskTurnpointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "TaskTurnpointsDlg.h"
#include "TaskDisplayDlg.h"
#include "TurnpointPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskTurnpointsDlg dialog


CTaskTurnpointsDlg::CTaskTurnpointsDlg(CWinscoreDoc *pDoc, CTask &cTask, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskTurnpointsDlg::IDD, pParent),
	m_pDoc(pDoc),
	m_cTask(cTask)
{

	//{{AFX_DATA_INIT(CTaskTurnpointsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaskTurnpointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskTurnpointsDlg)
	DDX_Control(pDX, IDC_RADII_TEXT, m_cRadiiText);
	DDX_Control(pDX, IDC_RADII_EDIT, m_cRadiiEdit);
	DDX_Control(pDX, IDC_TURNPOINT_EDIT, m_cTurnpointEdit);
	DDX_Control(pDX, IDC_TURNPOINT_LIST, m_cTurnpointListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskTurnpointsDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskTurnpointsDlg)
	ON_BN_CLICKED(IDC_TASK_PLANNER, OnTaskPlanner)
	ON_BN_CLICKED(IDC_SELECTPOINTSBUTTON, OnSelectpointsbutton)
	ON_EN_CHANGE(IDC_TURNPOINT_EDIT, OnChangeTurnpointEdit)
	ON_EN_CHANGE(IDC_RADII_EDIT, OnChangeTurnpointEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskTurnpointsDlg message handlers

BOOL CTaskTurnpointsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cTurnpointListCtrl.Initalize(m_pDoc->m_eUnits, m_pDoc->GetUnitsText(), &m_pDoc->m_turnpointArray, false, m_cTask.IsAreaTask() );

	m_cTurnpointListCtrl.ResetContent();
	m_cTurnpointListCtrl.SetStartGate(m_cTask.m_cStartGate);
	m_cTurnpointListCtrl.SetFinishGate(m_cTask.m_cFinishGate);

	for( int i=0; i<m_cTask.GetNumTurnpoints(); i++)
		{
		m_cTurnpointEdit.AddTurnpoint( m_cTask.GetTurnpointID(i) );
		if( m_cTask.IsAreaTask() )
			m_cRadiiEdit.AddTurnpoint( ConvertDistance(m_cTask.GetTurnpointRadius(i), SYSTEMUNITS, m_pDoc->m_eUnits) );
		else
			m_cRadiiEdit.AddTurnpoint( 1 );
		}

	m_cRadiiEdit.EnableWindow(m_cTask.IsAreaTask() );
	m_cRadiiText.EnableWindow(m_cTask.IsAreaTask() );

	RefreshTaskList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaskTurnpointsDlg::RefreshTaskList()
{	
	int aiTurnpoints[MAXTASKTPS];
	double adTurnpointRadii[MAXTASKTPS];
	for( int i=0; i<MAXTASKTPS; i++)  adTurnpointRadii[i]=1.;

	int nParsed=m_cTurnpointEdit.ParseTurnpoints( MAXTASKTPS, aiTurnpoints);
				m_cRadiiEdit.ParseTurnpointRadii( MAXTASKTPS, adTurnpointRadii);

	for( int i=0; i<MAXTASKTPS; i++) 
		{
		adTurnpointRadii[i]=min(adTurnpointRadii[i], 50.) ;
		adTurnpointRadii[i]=max(adTurnpointRadii[i], .5) ;
		}
	
	m_cTurnpointListCtrl.ResetContent();
	m_cTurnpointListCtrl.AddTurnpoint(m_cTask.m_cStartGate.GetPointID(),TPT_START, m_cTask.m_cStartGate.IsGPSCylinder()?(m_cTask.m_cStartGate.GetRadius()):(0));
	for( int i=0; i<nParsed; i++ )
		{
		m_cTurnpointListCtrl.AddTurnpoint(aiTurnpoints[i], TPT_CONTROL, ConvertDistance(adTurnpointRadii[i], m_pDoc->m_eUnits, SYSTEMUNITS) );
		}
	m_cTurnpointListCtrl.AddTurnpoint(m_cTask.m_cFinishGate.GetPointID(),TPT_FINISH, m_cTask.m_cFinishGate.IsGPSCylinder()?(m_cTask.m_cFinishGate.GetRadius()):(0));
	m_cTurnpointListCtrl.UpdateDistance();
}

void CTaskTurnpointsDlg::OnTaskPlanner() 
{
	CTask cTempTask;
	cTempTask=m_cTask;

	cTempTask.SetNumTurnpoints(m_cTurnpointListCtrl.GetNumTurnpoints());

	for( int i=0; i<m_cTurnpointListCtrl.GetNumTurnpoints(); i++)
		{
		cTempTask.SetTurnpointID(i, m_cTurnpointListCtrl.GetTurnpointID(i) );
		}

	if( cTempTask.IsAreaTask() )
		{
		for(  int i=0; i<m_cTurnpointListCtrl.GetNumTurnpoints(); i++)
			{
			cTempTask.SetTurnpointRadius(i, (float)ConvertDistance(m_cTurnpointListCtrl.GetTurnpointRadius(i), m_pDoc->m_eUnits, SYSTEMUNITS) );
			}
		}

	CTaskDisplayDlg dlg( &cTempTask, m_pDoc );
	dlg.DoModal();

/*
	CString cIDText;
	CPlannerDlg	dlg(m_pDoc, m_cTask.m_cStartGate, m_cTask.m_cFinishGate);
	if( dlg.DoModal()==IDOK )
		{
		m_cTurnpointEdit.Clear();
		for(int i=0; i<dlg.m_nTaskPoints; i++ )
			{
			CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray[dlg.m_aiTaskPoints[i]-1];
			if( !pcTurnpoint ) continue;

			if( (i==0 && pcTurnpoint->m_iID==m_cTask.m_cStartGate.GetPointID()) ||
			    (i==dlg.m_nTaskPoints-1 && pcTurnpoint->m_iID==m_cTask.m_cFinishGate.GetPointID())    ) continue;

			m_cTurnpointEdit.AddTurnpoint( pcTurnpoint->m_iID);
			}
		RefreshTaskList();
		}
		*/
}

void CTaskTurnpointsDlg::OnSelectpointsbutton() 
{
	CTurnpointPickerDlg dlg(m_pDoc->m_turnpointArray);	
	if( dlg.DoModal() ==IDOK )
		{
		if( dlg.m_pSelectedTurnpoint==NULL ) return;
		m_cTurnpointEdit.AddTurnpoint(dlg.m_pSelectedTurnpoint->m_iID);
		RefreshTaskList();
		}	
}

void CTaskTurnpointsDlg::OnChangeTurnpointEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	RefreshTaskList();
	
}

void CTaskTurnpointsDlg::OnOK() 
{
		m_cTask.SetNumTurnpoints(m_cTurnpointListCtrl.GetNumTurnpoints());

		for( int i=0; i<m_cTurnpointListCtrl.GetNumTurnpoints(); i++)
			{
			m_cTask.SetTurnpointID(i, m_cTurnpointListCtrl.GetTurnpointID(i) );
			}

		if( m_cTask.IsAreaTask() )
			{
			for( int  i=0; i<m_cTurnpointListCtrl.GetNumTurnpoints(); i++)
				{
				m_cTask.SetTurnpointRadius(i, (float)ConvertDistance(m_cTurnpointListCtrl.GetTurnpointRadius(i), m_pDoc->m_eUnits, SYSTEMUNITS) );
				}
			}

    if( m_cTask.IsAreaTask() )
		{
		// Check Turn Area Separation

		if (!m_cTask.CheckTurnAreaDistances(m_pDoc->m_turnpointArray) )
			{
			if( m_cTask.IsFAITask() )
				AfxMessageBox(_T("Invalid Area Task, check for 1 Km separation between specified areas."));
			else
			    AfxMessageBox(_T("Invalid Turn Area Task, check for 2 mile separation between turn areas."));
			}

		}
	CDialog::OnOK();
}

void CTaskTurnpointsDlg::OnChangeRadiiEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

