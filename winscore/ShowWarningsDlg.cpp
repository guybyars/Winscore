//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ShowWarningsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "flight.h"
#include "ResizingDialog.h"
#include "ShowWarningsDlg.h"
#include "global_prototypes.h"
#include "Contestant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowWarningsDlg dialog


CShowWarningsDlg::CShowWarningsDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CShowWarningsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowWarningsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    SetControlInfo(IDC_LISTCTRL,    RESIZE_HOR | RESIZE_VER);
	SetControlInfo(IDC_RECHECK_LATEST,  ANCHORE_BOTTOM);
	SetControlInfo(IDC_APPLY_PENALTY,   ANCHORE_BOTTOM);
	SetControlInfo(IDC_SECURITY_PASSED, ANCHORE_BOTTOM);
	SetControlInfo(IDC_CLEAR,			ANCHORE_BOTTOM);
	SetControlInfo(IDC_CLOSE,			ANCHORE_BOTTOM);
}


void CShowWarningsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowWarningsDlg)
	DDX_Control(pDX, IDC_LISTCTRL, m_cListCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RECHECK_LATEST, m_cLatestStartButton);
	DDX_Control(pDX, IDC_APPLY_PENALTY, m_cPenaltyButton);
	DDX_Control(pDX, IDC_SECURITY_PASSED, m_cSecurityButton);
	DDX_Control(pDX, IDC_CLEAR, m_cClearButton);
	DDX_Control(pDX, IDC_LISTCTRL, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(CShowWarningsDlg, CDialog)
	//{{AFX_MSG_MAP(CShowWarningsDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RECHECK_LATEST, &CShowWarningsDlg::OnBnClickedRecheckLatest)
	ON_BN_CLICKED(IDC_APPLY_PENALTY, &CShowWarningsDlg::OnBnClickedApplyPenalty)
	ON_BN_CLICKED(IDC_SECURITY_PASSED, &CShowWarningsDlg::OnBnClickedSecurityPassed)
	ON_BN_CLICKED(IDC_CLEAR, &CShowWarningsDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_CLOSE, &CShowWarningsDlg::OnBnClickedClose)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LISTCTRL, &CShowWarningsDlg::OnLvnItemchangingListctrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowWarningsDlg message handlers

BOOL CShowWarningsDlg::OnInitDialog() 
{

	CDialog::OnInitDialog();
	if( !m_pcFlight ) return TRUE;
	
	CString cWinText="Warnings for ";
	cWinText+=m_pcFlight->m_strPilot;

	SetWindowText(cWinText);

	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_CENTER;
	lvc.iSubItem=0;


	lvc.pszText = _T("Status");
	lvc.cx = 80;
	m_cListCtrl.InsertColumn(lvc.iSubItem++,&lvc);

	lvc.pszText = _T("Type");
	lvc.cx = 90;
	m_cListCtrl.InsertColumn(lvc.iSubItem++,&lvc);


	lvc.pszText = _T("Recommended Penalty");
	lvc.cx = 120;
	m_cListCtrl.InsertColumn(lvc.iSubItem++,&lvc);


	lvc.pszText = _T("Description");
	lvc.cx = 270;
	lvc.fmt =LVCFMT_LEFT;
	m_cListCtrl.InsertColumn(lvc.iSubItem++,&lvc);

	LoadWarningsFromFlight(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CShowWarningsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
		
}

void CShowWarningsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();
}



void CShowWarningsDlg::OnBnClickedRecheckLatest()
{
	int iItem=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(iItem<0 ) return;
	CWarning *pcWarning=(CWarning*)m_cListCtrl.GetItemData( iItem );
	if( pcWarning->GetType()!=eStartLatest &&  pcWarning->GetType()!=eStart ) return;

	double dRatio= m_pDoc->GetLogTurninRatio( m_pcFlight->GetStartTime(),  m_pcFlight->m_eClass);
	if( dRatio<.80 )
		{
		CString strWarn;
		strWarn.Format(_T("Only %2.0f%% of the logs have been turned in.  You should only use this function if 80%% or more logs have been scored.  Continue anyway?"),dRatio*100);
		int iAnswer=AfxMessageBox( strWarn, MB_YESNOCANCEL  );
		if( iAnswer!=IDYES ) return;
		}

  Rechecklateststart( m_pcFlight, m_pDoc, false, true);

  //We need to reload the warnings because an analysis would have deleted and recreated them, thereby invalidating the pointers in the list control
  LoadWarningsFromFlight(iItem);
}


void CShowWarningsDlg::OnBnClickedApplyPenalty()
{
	CPenality *pcPenalty=NULL;
	CString strMess;

	CString cReason, cCode;

	int iItem=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(iItem<0 ) return;
	CWarning *pcWarning=(CWarning*)m_cListCtrl.GetItemData( iItem );
	if( pcWarning->m_iPenalty==0 ) return;
	int iCode=-1;
	if(  pcWarning->GetType()==eStart			||
		 pcWarning->GetType()==eTurnpoint		||
		 pcWarning->GetType()==eFinish			||
		 pcWarning->GetType()==eAirspaceMinor	||
		 pcWarning->GetType()==eAirspaceMajor		)
		{

	if( pcWarning->GetType()==eStart ) 
		iCode=4;
	else if( pcWarning->GetType()==eTurnpoint)
		iCode=3;
	else if(  pcWarning->GetType()==eFinish	)
		iCode=2;
	else if(  pcWarning->GetType()==eAirspaceMinor)
		iCode=0;
	else if(  pcWarning->GetType()==eAirspaceMajor)
		iCode= -1;

	if( m_pcFlight->GetRollTime()==0 ) return;
/*
#define NUMCODES  10
static _TCHAR *_gszPredefinedCodes[NUMCODES] =
{
	_T("AS"),
	_T("AD"), 
	_T("FP"), 
	_T("TP"),  
	_T("SP"), 
	_T("FD"), 
	_T("TI"), 
	_T("UO"), 
	_T("UC"), 
	_T("WT"), 
};
static _TCHAR *_gszPredefinedDescriptions[NUMCODES] =
{
	_T("Airspace Violation"), 
	_T("Administrative penalty"), 
	_T("Finish Penalty"), 
	_T("Turnpoint penalty"),  
	_T("Start penalty"), 
	_T("Incomplete flight documentation"), 
	_T("Time interval (LTI/ FDI) exceeded"), 
	_T("Unsafe operation"), 
	_T("Unsportsmanlike conduct"), 
	_T("Weight violation"), 
};
*/
		if( pcWarning->GetType()==eAirspaceMajor )
			{
			pcPenalty=m_pDoc->m_PenalityList.Get( m_pcFlight->m_strCID, m_pcFlight->GetRollTime(), eContestPenalty);
			if( pcPenalty )
				{
				strMess.Format("A %d point Contest Penalty has already been applied to %s for this day.  Continue anyway?", (int)(pcPenalty->m_dPenality), m_pcFlight->m_strCID);
				int iAnswer=AfxMessageBox( strMess, MB_YESNOCANCEL | MB_ICONEXCLAMATION   );
				if( iAnswer!=IDYES ) return;
				}
			else
				{
				strMess.Format("Ok to apply a %d point  Airspace/Contest Penalty to %s for this day?  This will also result in zero distance for the day.", pcWarning->m_iPenalty, m_pcFlight->m_strCID);
				int iAnswer=AfxMessageBox( strMess, MB_YESNOCANCEL | MB_ICONQUESTION   );
				if( iAnswer!=IDYES ) return;
				}	
			cCode=CString("AS");
			cReason=CString("Airspace Penalty");
			pcPenalty=new CPenality( m_pcFlight->m_strCID,  m_pcFlight->GetRollTime(), eContestPenalty, cCode, cReason, (double)(pcWarning->m_iPenalty)  );
			}
		else
			{
			pcPenalty=m_pDoc->m_PenalityList.Get( m_pcFlight->m_strCID, m_pcFlight->GetRollTime(), eDayPoint);
			if( pcPenalty )
				{
				strMess.Format("A %d point penalty has already been applied to %s for this day.  Continue anyway?", (int)(pcPenalty->m_dPenality), m_pcFlight->m_strCID);
				int iAnswer=AfxMessageBox( strMess, MB_YESNOCANCEL | MB_ICONEXCLAMATION   );
				if( iAnswer!=IDYES ) return;
				}	
			else
				{
				strMess.Format("Ok to apply a %d point penalty to %s for this day?", pcWarning->m_iPenalty, m_pcFlight->m_strCID);
				int iAnswer=AfxMessageBox( strMess, MB_YESNOCANCEL | MB_ICONQUESTION   );
				if( iAnswer!=IDYES ) return;
				}	

			cCode=CString(_gszPredefinedCodes[iCode]);
			cReason=CString(_gszPredefinedDescriptions[iCode]);

			pcPenalty=new CPenality( m_pcFlight->m_strCID,  m_pcFlight->GetRollTime(), eDayPoint, cCode, cReason, (double)(pcWarning->m_iPenalty)  );

			}
		m_pDoc->m_PenalityList.AddToList(pcPenalty);

		CString strMess;
		strMess.Format("A %d point %s was applied to %s for this day.", pcWarning->m_iPenalty, cReason, m_pcFlight->m_strCID);
		AfxMessageBox( strMess, MB_ICONINFORMATION  );

		pcWarning->Clear();
		m_cListCtrl.SetItemText(iItem,0,"Cleared" );  
		m_pDoc->SetModifiedFlag();
		m_cPenaltyButton.EnableWindow(FALSE);
		}
}



void CShowWarningsDlg::OnBnClickedSecurityPassed()
{
	int iItem=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(iItem<0 ) return;

	CWarning *pcWarning=(CWarning*)m_cListCtrl.GetItemData( iItem );
	if( pcWarning->GetType()!=eSecurity) return;

	m_pcFlight->SetSecurityPassed(); 
	m_pDoc->LogPassed(m_pcFlight->m_strFileName);

	CString strMess;
	strMess.Format("Log security for %s was marked as passed and will not be checked again.", m_pcFlight->m_strCID);
	AfxMessageBox( strMess, MB_ICONINFORMATION  );

	pcWarning->Clear();
	m_cListCtrl.SetItemText(iItem,0,"Cleared" );  
	m_pDoc->SetModifiedFlag();
	m_cSecurityButton.EnableWindow(FALSE);

}


void CShowWarningsDlg::OnBnClickedClear()
{
	int iItem=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(iItem<0 ) return;
	CWarning *pcWarning=(CWarning*)m_cListCtrl.GetItemData( iItem );
	pcWarning->Clear();
	m_cListCtrl.SetItemText(iItem,0,"Cleared" );  
	m_pDoc->SetModifiedFlag();
}


void CShowWarningsDlg::OnBnClickedClose()
{
	CDialog::OnCancel();
}




void CShowWarningsDlg::OnLvnItemchangingListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if ( pNMLV->uNewState & LVIS_SELECTED ) 
		{
		CWarning *pcWarning=(CWarning*)pNMLV->lParam;
		if( !pcWarning ) return;

		m_cLatestStartButton.EnableWindow(	pcWarning->IsActive() && (pcWarning->GetType()==eStartLatest||pcWarning->GetType()==eStart));
		m_cPenaltyButton.EnableWindow(		pcWarning->IsActive() && pcWarning->m_iPenalty>0);
		m_cSecurityButton.EnableWindow(		pcWarning->IsActive() && pcWarning->GetType()==eSecurity);
		m_cClearButton.EnableWindow(		pcWarning->IsActive());
		}
}

void  CShowWarningsDlg::LoadWarningsFromFlight(int iSel)
	{
	TCHAR		buff[10];

	m_cListCtrl.DeleteAllItems();

	int nWarnings=m_pcFlight->NumberOfWarnings();

	if( nWarnings>0 )
		{
		for (int iItem=0; iItem<nWarnings; iItem++ )
			{
			CWarning *pcWarning=m_pcFlight->GetWarning(iItem);
			int iColumn=0;
			LV_ITEM lvi;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_PARAM;
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcWarning;
			m_cListCtrl.InsertItem(&lvi);
			m_cListCtrl.SetItemText(iItem,iColumn++,pcWarning->IsActive()?"Active":"Cleared" );  

			m_cListCtrl.SetItemText(iItem,iColumn++,pcWarning->GetTypeText() );  

			if(  pcWarning->m_iPenalty==0 )
				memset(buff,0,10);
			else
				_itot_s( pcWarning->m_iPenalty, buff, 10,10);
			m_cListCtrl.SetItemText(iItem,iColumn++,buff ); 

			m_cListCtrl.SetItemText(iItem,iColumn++,pcWarning->m_strWarning ); 
			}

		for(int i = 2;i < m_cListCtrl.GetHeaderCtrl()->GetItemCount();++i)
			m_cListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);		}

	m_cListCtrl.SetItemState(iSel, LVIS_SELECTED, LVIS_SELECTED);
}

