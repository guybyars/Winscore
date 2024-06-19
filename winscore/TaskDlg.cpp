//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "TaskList.h"
#include "TaskDlg.h"
#include "WindDlg.h"
#include "gatedlg.h"
#include "resizingdialog.h"
#include "Global_Prototypes.h"
#include "TaskTurnpointsDlg.h"
#include "TaskTimeDlg.h"
#include "DontShowDlg.h"
#include "TaskLibraryDlg.h"
#include "TaskDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool bApplied=false;
static void RefreshTaskList();
static bool bNag=true;

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg dialog


CTaskDlg::CTaskDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CTaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaskDlg)
	m_iStatus = -1;
	m_iType = -1;
	//}}AFX_DATA_INIT

	m_pPreselect=NULL;

    SetControlInfo(IDC_OK,				ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,			ANCHORE_BOTTOM);
    SetControlInfo(IDC_APPLY,			ANCHORE_BOTTOM);

    SetControlInfo(IDC_EXPORT,			ANCHORE_BOTTOM);
    SetControlInfo(IDC_IMPORT,			ANCHORE_BOTTOM);
    SetControlInfo(IDC_TASK_PLANNER2,	ANCHORE_BOTTOM);

    SetControlInfo(IDC_STATUS_LIST,		RESIZE_VER);
    SetControlInfo(IDC_WSVIEWCTRL1,		RESIZE_HOR		| RESIZE_VER);
}


void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskDlg)
	DDX_Control(pDX, IDC_START_GATE_BUTTON_B, m_c2ndStartGateButton);
	DDX_Control(pDX, IDC_2ND_GATE_CHECK, m_c2ndGateCheck);
	DDX_Control(pDX, IDC_TYPE_RADIO, m_cTypeRadio);
	DDX_Control(pDX, IDC_TASKTYPETEXT, m_cTaskTypeText);
	DDX_Control(pDX, IDC_FINISH_GATE_BUTTON, m_cFinishGateButton);
	DDX_Control(pDX, IDC_TIME_BUTTON, m_cTimeButton);
	DDX_Control(pDX, IDC_TURNPOINT_BUTTON, m_cTurnpointButton);
	DDX_Control(pDX, IDC_START_GATE_BUTTON, m_cStartGateButton);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
	DDX_Control(pDX, IDC_CLASS_COMBO, m_cClassCombo);
	DDX_Radio(pDX, IDC_STATUS_RADIO, m_iStatus);
	DDX_Radio(pDX, IDC_TYPE_RADIO, m_iType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WSVIEWCTRL1, m_cWSViewCtrl);
	DDX_Control(pDX, IDC_STATUS_LIST, m_cStatusListCtrl);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CResizingDialog)
	//{{AFX_MSG_MAP(CTaskDlg)
	ON_CBN_SELCHANGE(IDC_CLASS_COMBO, OnSelchangeClassCombo)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO, OnSelchangeDateCombo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_START_GATE_BUTTON, OnStartGateButton)
	ON_BN_CLICKED(IDC_FINISH_GATE_BUTTON, OnFinishGateButton)
	ON_BN_CLICKED(IDC_TYPE_RADIO, OnTypeRadio)
	ON_BN_CLICKED(IDC_TURNPOINT_BUTTON, OnTurnpointButton)
	ON_BN_CLICKED(IDC_TIME_BUTTON, OnTimeButton)
	ON_BN_CLICKED(IDC_STATUS_RADIO, OnStatusRadio)
	ON_BN_CLICKED(IDC_2ND_GATE_CHECK, On2ndGateCheck)
	ON_BN_CLICKED(IDC_MOD_RADIO, OnTypeRadio)
	ON_BN_CLICKED(IDC_AREA_RADIO, OnTypeRadio)
	ON_BN_CLICKED(IDC_FAI_RACE_RADIO, OnTypeRadio)
	ON_BN_CLICKED(IDC_FAI_ASSIGNED_AREA, OnTypeRadio)
	ON_BN_CLICKED(IDC_PRELIM_RADIO, OnStatusRadio)
	ON_BN_CLICKED(IDC_UNOFFICIAL_RADIO, OnStatusRadio)
	ON_BN_CLICKED(IDC_OFFICIAL_RADIO, OnStatusRadio)
	ON_BN_CLICKED(IDC_START_GATE_BUTTON_B, OnStartGateButtonB)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OK, &CTaskDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EXPORT, &CTaskDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_IMPORT, &CTaskDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_TASK_PLANNER2, &CTaskDlg::OnBnClickedTaskPlanner2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg message handlers

BOOL CTaskDlg::OnInitDialog() 
{
	CResizingDialog::OnInitDialog();
	

	m_cStatusListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;
	lvc.iSubItem=0;


	lvc.pszText = _T("Item");
	lvc.cx = 100;
	m_cStatusListCtrl.InsertColumn(lvc.iSubItem++,&lvc);

	lvc.pszText = _T("Value");
	lvc.cx = 240;
	m_cStatusListCtrl.InsertColumn(lvc.iSubItem++,&lvc);


	// Setup Combo Boxes
	m_pDoc->SetAvailableClasses(&m_cClassCombo);
	m_pDoc->LoadDateComboBox(m_cDateCombo);

	if( m_pPreselect ) 
		{
		for(int i=0; i<m_cClassCombo.GetCount(); i++)
			{
			if( m_pPreselect->m_eClass==(EClass)m_cClassCombo.GetItemData(i))
				{
				m_cClassCombo.SetCurSel(i);
				break;
				}
			}
		for( int i=0; i<m_cDateCombo.GetCount(); i++)
			{
			CTime *pTime=((CTime*)m_cDateCombo.GetItemData(i));
			if( DatesEqual ( m_pPreselect->m_cDate, *pTime)  )
				{
				m_cDateCombo.SetCurSel(i);
				break;
				}
			}
		m_cTask= *m_pPreselect;
		}
	else
		{
		CTask *pcTask=GetTask();
		if( pcTask ) 
			m_cTask= *pcTask;
		else
			InitializeTask();
		}

	bApplied=false;

	MaskForm();

	LoadWSView();

	return TRUE;
}


void CTaskDlg::OnSelchangeClassCombo() 
	{
	CTask *pcTask=GetTask();
	if( pcTask ) 
		m_cTask= *pcTask;
	else
		if( !bApplied ) InitializeTask();
	MaskForm();
	}

void CTaskDlg::OnSelchangeDateCombo() 
	{
	CTask *pcTask=GetTask();
	if( pcTask ) 
		{
		m_cTask= *pcTask;
		}
	else
		{
		// IF the user just pressed the apply button, he probably wants to create another, similar task.
		// So don't initialize
		if( !bApplied ) 
			InitializeTask();
		else
			{
			int iSel=m_cDateCombo.GetCurSel();
			CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
			m_cTask.m_cDate= *pcTime;
			}
		}
	MaskForm();	
	UpdateWSView();
	}

void CTaskDlg::MaskForm()
	{
	bool bDisable2Gates=GetWinscoreInt( ALLOWTWOGATES, 0 )==0;

	UpdateData(false);

	switch (m_cTask.m_eStatus)
		{
		case eNoContest:
			m_iStatus=0;
			break;
		case ePreliminary:
			m_iStatus=1;
			break;
		case eUnofficial:
			m_iStatus=2;
			break;
		case eOfficial:
			m_iStatus=3;
			break;
		}

	switch (m_cTask.m_eType)
		{
		case eAssigned:
			m_iType=0;
			break;
		case eModifiedAssigned:
			m_iType=1;
			break;
		case eTurnArea:
			m_iType=2;
			break;
		case eFAIRacing:
			m_iType=3;
			break;
		case eFAIAssignedArea:
			m_iType=4;
			break;
		}
//	m_iType=(ETaskType)m_cTask.m_eType;

	m_cStartGateButton.EnableWindow		( m_cTask.m_eStatus!=eNoContest );
	m_c2ndGateCheck.EnableWindow		( !bDisable2Gates && m_cTask.m_eStatus!=eNoContest );
	m_c2ndGateCheck.SetCheck			( !bDisable2Gates && m_cTask.m_b2ndGateActive );
	m_c2ndStartGateButton.EnableWindow	( m_cTask.m_eStatus!=eNoContest &&  m_cTask.m_b2ndGateActive );
	m_cFinishGateButton.EnableWindow	( m_cTask.m_eStatus!=eNoContest );
	m_cTimeButton.EnableWindow			( m_cTask.m_eStatus!=eNoContest );
	m_cTurnpointButton.EnableWindow		( m_cTask.m_eStatus!=eNoContest && (m_cTask.m_eType==eAssigned || m_cTask.m_eType==eFAIRacing || m_cTask.m_eType==eModifiedAssigned || m_cTask.IsAreaTask()) );
	m_cTaskTypeText.EnableWindow		( m_cTask.m_eStatus!=eNoContest );
	m_cTypeRadio.EnableWindow			( m_cTask.m_eStatus!=eNoContest );
	GetDlgItem( IDC_MOD_RADIO )->EnableWindow( m_cTask.m_eStatus!=eNoContest );
	GetDlgItem( IDC_AREA_RADIO )->EnableWindow( m_cTask.m_eStatus!=eNoContest );
	GetDlgItem( IDC_FAI_RACE_RADIO )->EnableWindow( m_cTask.m_eStatus!=eNoContest );
	GetDlgItem( IDC_FAI_ASSIGNED_AREA )->EnableWindow( m_cTask.m_eStatus!=eNoContest );
	
	if( m_cTask.m_b2ndGateActive )
		{
		m_cStartGateButton.SetWindowText(_T("Start Gate (A)..."));
		}
	else
		{
		m_cStartGateButton.SetWindowText(_T("Start Gate..."));
		}

	SetStatusText();

	UpdateData(false);
	}




void CTaskDlg::OnDestroy() 
{

	FreeDateComboBox(m_cDateCombo);
	CResizingDialog::OnDestroy();
		
}

void CTaskDlg::OnApply()
	{
	bApplied=true;
	ApplyDlg();
	}

bool CTaskDlg::ApplyDlg() 
{
	
	if( UpdateData()==0 ) return false;

    if( !ValidateTask() ) return false;

	int iSel=m_cDateCombo.GetCurSel();
	m_cSelectedDate= *(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	iSel=m_cClassCombo.GetCurSel();
	m_cTask.m_eClass = (EClass)m_cClassCombo.GetItemData(iSel);

	m_pcTaskList->AddToList(&m_cTask);
	return true;

}


void CTaskDlg::OnStartGateButton() 
{
	CGateDlg dlg( m_pDoc, CGateDlg::eStart, m_cTask.m_cStartGate, &m_cTask);
	if( dlg.DoModal()==IDOK )
		{
		m_cTask.m_cStartGate=dlg.m_cGate;
		WriteWinscoreInt(  szStartID,			m_cTask.m_cStartGate.GetPointID());
		WriteWinscoreInt(  szStartHeading,(int)m_cTask.m_cStartGate.GetHeading());
		WriteWinscoreInt(  szStartType,   (int)m_cTask.m_cStartGate.GetGateType());
		WriteWinscoreInt(  szStartCeiling,	m_cTask.m_cStartGate.GetHeight());
		WriteWinscoreDouble( szStartRadius,  ConvertDistance( m_cTask.m_cStartGate.GetRadius(), SYSTEMUNITS, eStatute)  );
		MaskForm();	
		UpdateWSView();
		}
}

void CTaskDlg::OnFinishGateButton() 
{
	CGateDlg dlg( m_pDoc, CGateDlg::eFinish, m_cTask.m_cFinishGate, &m_cTask);
	if( dlg.DoModal()==IDOK )
		{
		m_cTask.m_cFinishGate=dlg.m_cGate;
		WriteWinscoreInt(  szFinishHeading,(int)m_cTask.m_cFinishGate.GetHeading());
		WriteWinscoreInt(  szFinishType, (int)m_cTask.m_cFinishGate.GetGateType());
		WriteWinscoreInt(  szFinishBase,		m_cTask.m_cFinishGate.GetBase());
		WriteWinscoreInt(  szFinishID,		m_cTask.m_cFinishGate.GetPointID());
		WriteWinscoreDouble( szFinishRadius, ConvertDistance( m_cTask.m_cFinishGate.GetRadius(), SYSTEMUNITS, eStatute)  );
		MaskForm();	
		UpdateWSView();
		}
}

bool CTaskDlg::IsContestDay()
		{
		return (EStatus)m_iStatus==eNoContest;
		}

void CTaskDlg::SetTaskList(CTaskList * pcTaskList)
{
m_pcTaskList=pcTaskList;
}

void CTaskDlg::OnTypeRadio() 
{
	UpdateData(true);


	switch (m_iType)
		{
		case 0:
			m_cTask.m_eType=eAssigned;
			for( int i=0; i<MAXTASKTPS; i++) m_cTask.SetTurnpointRadius(i,(float)ConvertDistance(1, eStatute, SYSTEMUNITS ));
			break;
		case 1:
			m_cTask.m_eType=eModifiedAssigned;
			for( int i=0; i<MAXTASKTPS; i++) m_cTask.SetTurnpointRadius(i,(float)ConvertDistance(1, eStatute, SYSTEMUNITS ));
			break;
		case 2:
			m_cTask.m_eType=eTurnArea;
			break;
		case 3:
			m_cTask.m_eType=eFAIRacing;
			for( int i=0; i<MAXTASKTPS; i++) m_cTask.SetTurnpointRadius(i,(float)ConvertDistance(1, eStatute, SYSTEMUNITS ));
			break;
		case 4:
			m_cTask.m_eType=eFAIAssignedArea;
			break;
		}

	MaskForm();
	UpdateWSView();
	
}

void CTaskDlg::OnTurnpointButton() 
{

	if( m_cTask.m_cStartGate.GetPointID()==0 ||
		!m_pDoc->m_turnpointArray[m_cTask.m_cStartGate.GetPointID()-1]) 
		{
		AfxMessageBox(_T("You must first define the start gate."));
		return;
		}

	if(  m_cTask.m_b2ndGateActive && 
		(m_cTask.m_cStartGate2.GetPointID()==0 || !m_pDoc->m_turnpointArray[m_cTask.m_cStartGate2.GetPointID()-1]) ) 
		{
		AfxMessageBox(_T("You must first define the start gate B."));
		return;
		}

	if( m_cTask.m_cFinishGate.GetPointID()==0 ||
		!m_pDoc->m_turnpointArray[m_cTask.m_cFinishGate.GetPointID()-1]) 
		{
		AfxMessageBox(_T("You must first define the finish gate."));
		return;
		}

	CTaskTurnpointsDlg dlg( m_pDoc, m_cTask );
	if( dlg.DoModal()==IDOK )
		{
		MaskForm();	
		UpdateWSView();
		}
	
}

void CTaskDlg::OnTimeButton() 
{
	CTaskTimeDlg dlg( m_pDoc, m_cTask );
	if( dlg.DoModal()==IDOK )
		MaskForm();	

	
}

void CTaskDlg::OnStatusRadio() 
{
	UpdateData(true);
	CTask *pcTask=GetTask();
	if( pcTask ) 
		{
		m_cTask= *pcTask;
		}
	else
		{
		InitializeTask();
		}

	switch (m_iStatus)
		{
		case 0:
			m_cTask.m_eStatus=eNoContest;
			break;
		case 1:
			m_cTask.m_eStatus=ePreliminary;
			break;
		case 2:
			m_cTask.m_eStatus=eUnofficial;
			break;
		case 3:
			{
			m_cTask.m_eStatus=eOfficial;
			CDontShowDlg dsdlg(_T("TASK_NOW_OFFICIAL"),_T("This day is now Official for this class.\nScores are locked and will not be recalculated."),
						IDI_INFORMATION);
			dsdlg.DoModalConditional();

			//AfxMessageBox("This day is now Official for this class.\nScores are locked and will not be recalculated.");	
			break;
			}
		}
	UpdateData(false);
	MaskForm();	
}

CTask* CTaskDlg::GetTask()
	{
	int iSel=m_cDateCombo.GetCurSel();
	if( iSel<0 ) return NULL;
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
    
	iSel=m_cClassCombo.GetCurSel();
	if( iSel<0 ) return NULL;
	EClass eClass = (EClass)m_cClassCombo.GetItemData(iSel);

	return m_pcTaskList->GetByDateClass( *pcTime, eClass );

	}

void CTaskDlg::InitializeTask()
	{
	m_cTask=CTask();


	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	m_cTask.m_cDate= *pcTime;
	m_cTask.m_cStartGate.SetRadius(ConvertDistance( GetWinscoreDouble( szStartRadius, 5.0), eStatute, SYSTEMUNITS ));
	m_cTask.m_cFinishGate.SetRadius( ConvertDistance( GetWinscoreDouble( szFinishRadius, 2.0), eStatute, SYSTEMUNITS ));

	m_cTask.m_cStartGate.SetHeight(		GetWinscoreInt(  szStartCeiling, 5000));
	m_cTask.m_cStartGate.SetGateType((EGateType)GetWinscoreInt(  szStartType, (int)eCylinder));
	m_cTask.m_cStartGate.SetHeading(	GetWinscoreInt(  szStartHeading, 0));

	m_cTask.m_cStartGate.SetPointID(	GetWinscoreInt(  szStartID, 1));
	m_cTask.m_cStartGate.SetFirstLegAtExit(true /*m_pDoc->m_eContest==eRegional?true:false*/);

	// Make sure this Start point is valid.
	CTurnpoint* pStart=m_pDoc->m_turnpointArray.GetAt(m_cTask.m_cStartGate.GetPointID()-1);
	if( !pStart || !pStart->HasAttribute(TPT_START) )
		{
		CTurnpoint* pTpt=m_pDoc->m_turnpointArray.GetFirstByAttribute(TPT_START);
		if( pTpt )
			m_cTask.m_cStartGate.SetPointID(pTpt->GetID());
		}

	m_cTask.m_cStartGate2=m_cTask.m_cStartGate;

	m_cTask.m_cFinishGate.SetBase(		GetWinscoreInt(  szFinishBase, 200));
	m_cTask.m_cFinishGate.SetGateType((EGateType)GetWinscoreInt(  szFinishType, (int)eCylinder));
	m_cTask.m_cFinishGate.SetHeading(	GetWinscoreInt(  szFinishHeading, 0));
	m_cTask.m_cFinishGate.SetPointID(	GetWinscoreInt(  szFinishID, 1));



	// Make sure this Finish point is valid.
	CTurnpoint* pFinish=m_pDoc->m_turnpointArray.GetAt(m_cTask.m_cFinishGate.GetPointID()-1);
	if( !pFinish || !pFinish->HasAttribute(TPT_FINISH) )
		{
		CTurnpoint* pTpt=m_pDoc->m_turnpointArray.GetFirstByAttribute(TPT_FINISH);
		if( pTpt )
			m_cTask.m_cFinishGate.SetPointID(pTpt->GetID());
		}

	for( int i=0; i<MAXTASKTPS; i++) m_cTask.SetTurnpointID(i,0);
	for( int i=0; i<MAXTASKTPS; i++) m_cTask.SetTurnpointRadius(i,(float)ConvertDistance(1, eStatute, SYSTEMUNITS ));

	int iOpenHours		= GetWinscoreInt(  szOpenHours, 13);
	int iOpenMinutes	= GetWinscoreInt(  szOpenMinutes, 0);
	int iMinPostMinutes	= GetWinscoreInt(  szPostMinutes, 0);
	int iMinPostHours	= GetWinscoreInt(  szPostHours, 2 );

	m_cTask.m_cGateOpenTime  =CTime(pcTime->GetYear(),pcTime->GetMonth(),pcTime->GetDay(), iOpenHours, iOpenMinutes, 0);
	m_cTask.m_cPostTime	   =CTimeSpan(0, iMinPostHours, iMinPostMinutes, 0);

	}

void CTaskDlg::SetStatusText()
{
	LV_ITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = 0;
	lvi.lParam = (LPARAM)0;	
	CString strStatusText,strSpeedText;
	m_cStatusListCtrl.DeleteAllItems();


	int iItem=0;
	if( m_cTask.m_eStatus!=eNoContest )
		{
		lvi.iItem = iItem;
		m_cStatusListCtrl.InsertItem(&lvi);
		m_cStatusListCtrl.SetItemText(iItem,0,"Turnpoints" );  

		strStatusText=m_cTask.TurnpointText();
		m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );  


		double dHours=(double)m_cTask.m_cPostTime.GetTotalSeconds()/(60.*60);
		if( m_cTask.m_eType==eAssigned || m_cTask.m_eType==eFAIRacing )
			{
			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,"Distance" );  

			strStatusText=m_cTask.DistanceText(m_pDoc->m_turnpointArray, m_pDoc->m_eUnits);
			strStatusText+=_T(" ")+m_pDoc->GetUnitsText();
			m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );  
			}
		if( m_cTask.m_eType==eModifiedAssigned )
			{
			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,"Distance" );  

			strStatusText=m_cTask.DistanceText(m_pDoc->m_turnpointArray, m_pDoc->m_eUnits);
			strStatusText+=_T(" ")+m_pDoc->GetUnitsText();
			if(dHours>0.0 )
				{
				double dSpeed=m_cTask.GetDistance(m_pDoc->m_turnpointArray, m_pDoc->m_eUnits)/dHours;
				strSpeedText.Format("   -  %5.2lf %s", dSpeed, UnitsSpeedText(m_pDoc->m_eUnits));
				strStatusText+=strSpeedText;
				}
			m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );  
			}
		else if( m_cTask.IsAreaTask() ) 
			{
			double dMin;
			double dNominal; 
			double dMax;
			m_cTask.GetTurnAreaDistances(	m_pDoc->m_turnpointArray, dMin, dNominal, dMax);

			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,"Minimum Distance" );  
			strStatusText.Format(_T("%5.2lf"),ConvertDistance( dMin, SYSTEMUNITS, m_pDoc->m_eUnits), m_pDoc->GetUnitsText());
			strStatusText+=_T(" ")+m_pDoc->GetUnitsText();
			if(dHours>0.0 )
				{
				double dSpeed=dMin/dHours;
				strSpeedText.Format("   -  %5.2lf %s",ConvertDistance( dSpeed, SYSTEMUNITS, m_pDoc->m_eUnits),UnitsSpeedText(m_pDoc->m_eUnits));
				strStatusText+=strSpeedText;
				}
			m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );  

			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,"Nominal Distance" );  
			strStatusText.Format(_T("%5.2lf"),ConvertDistance( dNominal, SYSTEMUNITS, m_pDoc->m_eUnits), m_pDoc->GetUnitsText());
			strStatusText+=_T(" ")+m_pDoc->GetUnitsText();
			if(dHours>0.0 )
				{
				double dSpeed=dNominal/dHours;
				strSpeedText.Format("   -  %5.2lf %s",ConvertDistance( dSpeed, SYSTEMUNITS, m_pDoc->m_eUnits),UnitsSpeedText(m_pDoc->m_eUnits));
				strStatusText+=strSpeedText;
				}
			m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );  

			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,"Maximum Distance" );  
			strStatusText.Format(_T("%5.2lf"),ConvertDistance( dMax, SYSTEMUNITS, m_pDoc->m_eUnits), m_pDoc->GetUnitsText());
			strStatusText+=_T(" ")+m_pDoc->GetUnitsText();
			if(dHours>0.0 )
				{
				double dSpeed=dMax/dHours;
				strSpeedText.Format("   -  %5.2lf %s",ConvertDistance( dSpeed, SYSTEMUNITS, m_pDoc->m_eUnits),UnitsSpeedText(m_pDoc->m_eUnits));
				strStatusText+=strSpeedText;
				}
			m_cStatusListCtrl.SetItemText(iItem++,1,strStatusText );
			}

		if( m_cTask.IsTimedTask() )
			{
			lvi.iItem = iItem;
			m_cStatusListCtrl.InsertItem(&lvi);
			m_cStatusListCtrl.SetItemText(iItem,0,_T("Min Time :") );  
			m_cStatusListCtrl.SetItemText(iItem++,1,m_cTask.MinTimeText() );
			}

		lvi.iItem = iItem;
		m_cStatusListCtrl.InsertItem(&lvi);
		m_cStatusListCtrl.SetItemText(iItem,0,_T("Start Gate") );  
		m_cStatusListCtrl.SetItemText(iItem++,1,m_cTask.StartGateText(m_pDoc->m_turnpointArray ) );

		lvi.iItem = iItem;
		m_cStatusListCtrl.InsertItem(&lvi);
		m_cStatusListCtrl.SetItemText(iItem,0,_T("Start Gate Ceiling") );  
		m_cStatusListCtrl.SetItemText(iItem++,1,m_cTask.GetStartCeilingText() );

		lvi.iItem = iItem;
		m_cStatusListCtrl.InsertItem(&lvi);
		m_cStatusListCtrl.SetItemText(iItem,0,_T("Finish Gate") );  
		m_cStatusListCtrl.SetItemText(iItem++,1,m_cTask.FinishGateText(m_pDoc->m_turnpointArray) );

		lvi.iItem = iItem;
		m_cStatusListCtrl.InsertItem(&lvi);
		m_cStatusListCtrl.SetItemText(iItem,0,_T("Finish Gate Base") );  
		m_cStatusListCtrl.SetItemText(iItem++,1,m_cTask.GetFinishBaseText() );

		}

}

void CTaskDlg::On2ndGateCheck() 
{
	m_c2ndGateCheck.GetCheck()?(m_cTask.m_b2ndGateActive=true):(m_cTask.m_b2ndGateActive=false);
	MaskForm();	
}

void CTaskDlg::OnStartGateButtonB() 
{
	CGateDlg dlg( m_pDoc, CGateDlg::eStart, m_cTask.m_cStartGate2, &m_cTask);
	if( dlg.DoModal()==IDOK )
		{
		m_cTask.m_cStartGate2=dlg.m_cGate;
		WriteWinscoreInt(  szStartID,			m_cTask.m_cStartGate2.GetPointID());
		WriteWinscoreInt(  szStartHeading,(int)m_cTask.m_cStartGate2.GetHeading());
		WriteWinscoreInt(  szStartType,  (int)m_cTask.m_cStartGate2.GetGateType());
		WriteWinscoreInt(  szStartCeiling,	m_cTask.m_cStartGate2.GetHeight());
		WriteWinscoreDouble( szStartRadius,  ConvertDistance( m_cTask.m_cStartGate2.GetRadius(), SYSTEMUNITS, eStatute)  );
		MaskForm();	
		}	
}


void CTaskDlg::OnBnClickedOk()
{
	if( ApplyDlg() ) 
		{
		CResizingDialog::OnOK();
		}	
}


bool CTaskDlg::ValidateTask()
	{

//	if( m_cTask.IsFAITask() && bNag) 
//		{
//		AfxMessageBox( "Notice: The FAI tasks are experimental and not to be used in a SSA contest without a waiver", MB_OK  );
//		bNag=false;
//		}

	if( m_cTask.m_eType==eAssigned || 
        m_cTask.m_eType==eFAIRacing ||
		m_cTask.m_eType==eModifiedAssigned || 
		m_cTask.IsAreaTask() )
		{
		if( m_cTask.GetNumTurnpoints()>MAXTASKTPS )
			{
			// Too Many turnpoints
			AfxMessageBox( IDS_ERR_TASK_MAX_TURNPOINTS, MB_OK  );
			return false;
			}
		if( m_cTask.m_eType!=eModifiedAssigned && m_cTask.GetNumTurnpoints()==0 )
			{
			//Warning no turnpoints
			if( m_cTask.m_eType==eAssigned || m_cTask.m_eType==eFAIRacing )
				AfxMessageBox( IDS_ERR_TASK_NO_TURNPOINTS, MB_OK  );
			else if( m_cTask.IsAreaTask()  )
				AfxMessageBox( _T("No control points selected for the area task"), MB_OK  );
			else
				AfxMessageBox( _T("No turnpoints selected for the modified assigned task"), MB_OK  );
			return false;
			}

		CTurnpoint* pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(m_cTask.m_cStartGate.GetPointID()-1);
		if( pcTurnpoint )
			{
			if( (pcTurnpoint->m_iAttribute & TPT_START)==0 )
				{
				// First turnpoint must be start point
				 if( AfxMessageBox( _T("Start Gate point should have \"Start\" attribute"), MB_OKCANCEL )!=IDOK ) return false;
				}
			}
		pcTurnpoint=m_pDoc->m_turnpointArray.GetAt(m_cTask.m_cFinishGate.GetPointID()-1);
		if( pcTurnpoint )
			{
			if( (m_cTask.m_eType==eAssigned || m_cTask.m_eType==eFAIRacing || m_cTask.IsAreaTask() ) && (pcTurnpoint->m_iAttribute & TPT_FINISH)==0 )
				{
				// Last turnpoint must be finish point
				 if( AfxMessageBox( _T("Finish Gate point should have \"Finish\" attribute"), MB_OKCANCEL )!=IDOK ) return false;
				}
			}

		}

	if( m_cTask.IsTimedTask()  )
		{

		if( m_cTask.m_cPostTime.GetTotalSeconds()==0 )
			{
			AfxMessageBox( _T("No minimum task time entered.")  );
			return false;
			}
		}

	if(  m_cTask.m_b2ndGateActive && 
		(m_cTask.m_cStartGate.GetPointID()==m_cTask.m_cStartGate2.GetPointID() ) )
		{
		AfxMessageBox( _T("A and B gates must not be located at the same point."), MB_OK  );
		return false;
		}

    if( m_cTask.IsAreaTask() )
		{
		// Check Turn Area Separation
		CString strError;
		if (!m_cTask.CheckTurnAreaDistances(m_pDoc->m_turnpointArray,strError,m_pDoc->m_eUnits) )
			{
			if( m_cTask.IsFAITask() )
				AfxMessageBox(strError+_T(", invalid  Area Task, check for 1 Km separation between specified areas."));
			else
				AfxMessageBox(strError+_T(", invalid  Area Task, check for 2 mile separation between specified areas."));
			}
		}
	return true;
	}

void CTaskDlg::OnBnClickedExport()
{
	CString strFileName;
	if( UpdateData()==0 ) return;

    if( !ValidateTask() ) return;
	CTaskList cList;
	cList.AddTail(new CTask(&m_cTask));
	cList.ExportToTaskLibrary();
	}



void CTaskDlg::OnBnClickedImport()
{
	CTaskLibraryDlg dlg;
	dlg.m_pDoc=m_pDoc;
	if( dlg.DoModal()==IDOK )
		{
		if( !dlg.m_pTask )
			{
			AfxMessageBox(_T("Import Failed, selected file was corrupt or invalid."),MB_ICONEXCLAMATION);
			}
		else
			{
			//Don't use the class and date and task open time from the imported task, save and reset.
			EClass     eClass=m_cTask.m_eClass;
			CTime      cDate =m_cTask.m_cDate;
			CTime      cTime =m_cTask.m_cGateOpenTime;

			m_cTask= *(dlg.m_pTask);

			m_cTask.m_eClass=eClass;
			m_cTask.m_cDate=cDate;
	        m_cTask.m_cGateOpenTime=cTime;
			m_cTask.m_eStatus=ePreliminary;  // Imported tasks are always prelm.

			MaskForm();
			SetStatusText();
			UpdateData(false);
			UpdateWSView();
			}
		}	
}


void CTaskDlg::OnBnClickedTaskPlanner2()
{
	if( UpdateData()==0 ) return;

	CTaskDisplayDlg dlg( &m_cTask, m_pDoc );
	dlg.DoModal();
}

void CTaskDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	
}

void CTaskDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}
void CTaskDlg::LoadWSView() 
	{
	m_cWSViewCtrl.ClearTask();

	float fTurnpointRadius		= (float)GetWinscoreDouble( INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	float fTurnpointOuterRadius	= (float)GetWinscoreDouble( OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );
	m_cWSViewCtrl.SetDisplayOptionD(WSO_INNER_RADIUS, fTurnpointRadius);
	m_cWSViewCtrl.SetDisplayOptionD(WSO_OUTER_RADIUS, fTurnpointOuterRadius);
	m_cWSViewCtrl.SetDisplayOption(WSC_UNITS, m_pDoc->m_eUnits);

	// Add the control points
	m_pDoc->m_turnpointArray.AddToControl(m_cWSViewCtrl);

	m_cTask.AddToControl(m_pDoc->m_turnpointArray, m_cWSViewCtrl );

	// Add the SUA Stuff
	BOOL bSUADisplay = GetWinscoreInt( SUADISPLAY, DEFAULT_SUADISPLAY );
	if( bSUADisplay )
		{
		CString strAirspaceFilename=GetWinscoreString( SUADISPLAYFILE, _T("") );
		CStringArray strAirspaceArray;
		int nSections=LoadSUA(m_pDoc->m_turnpointArray);
		if( nSections>0  ) 
			{
			m_cWSViewCtrl.SetColor(WSC_AIRSPACE_COLOR, RGB(255,16,16));
			for( int i=0; i<GetSUAArray()->GetSize(); i++ )
				{
				m_cWSViewCtrl.AddSUAirspace( GetSUAArray()->GetAt(i)->m_strOriginalText );
				}
			}
		}
	
	m_cWSViewCtrl.AutoscaleTask();
	}

void CTaskDlg::UpdateWSView() 
	{
	m_cWSViewCtrl.ClearTask();
	m_cTask.AddToControl(m_pDoc->m_turnpointArray, m_cWSViewCtrl );
	m_cWSViewCtrl.AutoscaleTask();
	}