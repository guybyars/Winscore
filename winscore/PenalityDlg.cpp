//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PenalityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "PenalityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




static BOOL  fModified=FALSE;

/////////////////////////////////////////////////////////////////////////////
// CPenalityDlg dialog


CPenalityDlg::CPenalityDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CPenalityDlg::IDD, pParent)
	, m_strPreselectCID(_T(""))
{
	//{{AFX_DATA_INIT(CPenalityDlg)
	m_dDailyPenaligy = 0;
	m_cExplaination = _T("");
	m_cPointsOrPercent = _T("Points");
	m_strPenalityCode = _T("");
	//}}AFX_DATA_INIT


	m_iSortColumn=1;

    SetControlInfo(IDC_PENALITY_LIST,		RESIZE_HOR		| RESIZE_VER);
 	SetControlInfo(IDCANCEL,	ANCHORE_BOTTOM );
 	SetControlInfo(IDC_DELETE,	ANCHORE_BOTTOM );
 	SetControlInfo(IDC_APPLY,	ANCHORE_BOTTOM );
 	SetControlInfo(IDOK,		ANCHORE_BOTTOM );


}


void CPenalityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPenalityDlg)
	DDX_Control(pDX, IDC_EXPLAIN, m_ctlExplaination);
	DDX_Control(pDX, IDC_CODE_COMBO, m_cPenalityCode);
	DDX_Control(pDX, IDC_PERCENT_OR_POINTS, m_cPercentOrPointsText);
	DDX_Control(pDX, IDC_DAILY_PENALITY, m_cDailyPenaligy);
	DDX_Control(pDX, IDC_TYPE_COMBO, m_cTypeCombo);
	DDX_Control(pDX, IDC_CONTEST_COMBO, m_cContestNoCombo);
	DDX_Control(pDX, IDC_PENALITY_LIST, m_cPenalityList);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_DATE, m_cDateCombo);
	DDX_Control(pDX, IDC_APPLY, m_cApply);
	DDX_Text(pDX, IDC_DAILY_PENALITY, m_dDailyPenaligy);
	DDX_Text(pDX, IDC_EXPLAIN, m_cExplaination);
	DDX_Text(pDX, IDC_PERCENT_OR_POINTS, m_cPointsOrPercent);
	DDX_CBString(pDX, IDC_CODE_COMBO, m_strPenalityCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPenalityDlg, CDialog)
	//{{AFX_MSG_MAP(CPenalityDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_CBN_SELCHANGE(IDC_TYPE_COMBO, OnSelchangeTypeCombo)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_EN_CHANGE(IDC_CODE, OnChangeCode)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PENALITY_LIST, OnItemchangedPenalityList)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_KEYDOWN, IDC_PENALITY_LIST, OnKeydownPenalityList)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CODE_COMBO, OnSelchangeCodeCombo)
	ON_CBN_EDITCHANGE(IDC_CODE_COMBO, OnEditchangeCodeCombo)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PENALITY_LIST, OnColumnclickPenalityList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenalityDlg message handlers

/*
   TP - turnpoint penalty
   SP - start penalty
   UO - unsafe operation
   UC - unsportsmanlike conduct
   AD - administrative penalty
   FD - incomplete flight documentation
   WT - weight violation
   LC - revised landing card
   TI - time interval (LTI/ FDI) exceeded
   ST - STI violation
   MD - scored distance less than SMTD
   AB - airfield bonus awarded
*/

void CPenalityDlg::OnApply() 
{
	CPenality  cPenality;
	int iSel;

	if( UpdateData()==0 )return;

	iSel=m_cContestNoCombo.GetCurSel();
	ASSERT(iSel>=0);
	CContestant* pcContestant=(CContestant*)m_cContestNoCombo.GetItemDataPtr(iSel);
	if( !pcContestant )
		{
		//Warning invalid contest ID
		AfxMessageBox( IDS_ERR_CONTESTANT_NOT_FOUND, MB_OK  );
		m_cContestNoCombo.SetFocus();
		return;
		}

	iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	cPenality.m_cContestNo		=pcContestant->m_strContestNo;
	cPenality.m_ePenality=(EPenality)m_cTypeCombo.GetCurSel();
	cPenality.m_cDate			=*pcTime;
	cPenality.m_cPenalityCode	=m_strPenalityCode;
	cPenality.m_cPenalityReason	=m_cExplaination;
	cPenality.m_dPenality		=m_dDailyPenaligy;

	if( cPenality.m_cPenalityCode=="" ||
		cPenality.m_cPenalityReason==""  )
		{
		AfxMessageBox( NO_CODE_OR_REASON, MB_OK  );
		return;
		}

	if( ( cPenality.m_ePenality==eDayPoint ||
		  cPenality.m_ePenality==eDayPercent  ||
		  cPenality.m_ePenality==eContestPenalty     ) &&
		cPenality.m_dPenality==0)
		{
		AfxMessageBox( ZERO_PENALITY_POINTS, MB_OK   );
		return;
		}

	m_pcPenalityList->AddToList(&cPenality);
	fModified=TRUE;

	InitializeValues();
	fModified=TRUE;
	UpdateData(FALSE);
	UpdateListBox();
	m_cContestNoCombo.SetFocus();
	m_cPenalityCode.SetCurSel(-1);
	m_cApply.EnableWindow(FALSE);
}


void CPenalityDlg::OnSelchangeTypeCombo() 
{
 	MaskForm();	
}

void CPenalityDlg::OnOK() 
{
	
	CDialog::OnOK();
}

void CPenalityDlg::OnDelete() 
{
	int iSel = m_cPenalityList.GetNextItem( -1, LVNI_SELECTED );
	if( iSel<0) return;
	CPenality* pcPenality=(CPenality*)m_cPenalityList.GetItemData(iSel);
	if(pcPenality==NULL) return;

	m_pcPenalityList->RemoveByPointer(pcPenality);
	fModified=TRUE;
	//m_cDelete.EnableWindow(FALSE);
	InitializeValues();
	UpdateListBox();
	//iSel = m_cPenalityList.GetNextItem( iSel, LVNI_SELECTED );
	m_cPenalityList.SetItemState(iSel, LVNI_SELECTED, LVNI_SELECTED  );
	m_cPenalityList.SetItemState(iSel, LVNI_FOCUSED, LVNI_FOCUSED  );
	m_cPenalityList.SetSelectionMark(iSel);
}

BOOL CPenalityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc->LoadDateComboBox(m_cDateCombo);
	m_cDelete.EnableWindow(FALSE);
	m_cApply.EnableWindow(FALSE);
	m_pDoc->LoadContestantComboBox(m_cContestNoCombo);
	m_cTypeCombo.SetCurSel(0);
	m_pcPenalityList->CreateControlColumns(m_cPenalityList);

	m_ePenality=eDayPoint;

	m_cPenalityList.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	for( int iCode=0; iCode<NUMCODES; iCode++)
		{
		m_cPenalityCode.AddString(_gszPredefinedCodes[iCode]);
		}

	if( m_strPreselectCID.GetLength()>0 )
		{
		for (int i=0; i<m_cContestNoCombo.GetCount(); i++)
			{
			CContestant* pcContestant=(CContestant*)m_cContestNoCombo.GetItemDataPtr(i);
			if(pcContestant->m_strContestNo==m_strPreselectCID )
				{
				m_cContestNoCombo.SetCurSel(i);
				break;
				}
			}
		}

    UpdateListBox();

	MaskForm();
	fModified=FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPenalityDlg::UpdateListBox()
	{
	m_pcPenalityList->LoadPenalityList( m_cPenalityList );
	m_cPenalityList.SortItems( ComparePenalty,m_iSortColumn );

	}



void CPenalityDlg::MaskForm()
{
	m_ePenality=(EPenality)m_cTypeCombo.GetCurSel();

	if( m_ePenality==eDayPoint )
		{
		m_cPercentOrPointsText.EnableWindow(TRUE);
		m_cDailyPenaligy.EnableWindow(TRUE);
		m_cPointsOrPercent="Points";
		}
	else if( m_ePenality==eDayPercent )
		{
		m_cPercentOrPointsText.EnableWindow(TRUE);
		m_cDailyPenaligy.EnableWindow(TRUE);
		m_cPointsOrPercent="Percent";
		}
	else if( m_ePenality==eContestPenalty )
		{
		m_cPercentOrPointsText.EnableWindow(TRUE);
		m_cDailyPenaligy.EnableWindow(TRUE);
		m_cPointsOrPercent="Points";
		}
	else if(	m_ePenality==eComment			)
		{
		m_cPercentOrPointsText.EnableWindow(FALSE);
		m_cDailyPenaligy.EnableWindow(FALSE);
		m_cPointsOrPercent=" ";
		}
	UpdateData(FALSE);
}

void CPenalityDlg::OnChangeCode() 
{
	m_cApply.EnableWindow(TRUE);	
}

void CPenalityDlg::OnItemchangedPenalityList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ( pNMListView->uNewState & LVIS_SELECTED )
		{ 
		m_cDelete.EnableWindow(TRUE);
		InitializeValues();
		}	
	*pResult = 0;
}

void CPenalityDlg::InitializeValues()
{
	m_cExplaination="";
	UpdateData(FALSE);
}


void CPenalityDlg::OnDestroy() 
{
	FreeDateComboBox(m_cDateCombo);
	CDialog::OnDestroy();
}

void CPenalityDlg::OnKeydownPenalityList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if( pLVKeyDow->wVKey==DELETE_KEY ) 
		{
		OnDelete();
		}	

	*pResult = 0;
}

void CPenalityDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CPenalityDlg::OnCancel() 
{
	if( fModified )
		{	
		int iAnswer=AfxMessageBox( _T("Save changes to penalities?"), MB_YESNOCANCEL  );
		if( iAnswer==IDYES )
			CDialog::OnOK();
		else if( iAnswer==IDNO )
			CDialog::OnCancel();
		else if( iAnswer==IDCANCEL)
			return;
		}
	else
		CDialog::OnCancel();
}

void CPenalityDlg::OnSelchangeCodeCombo() 
{
	int iSel=m_cPenalityCode.GetCurSel();
	if( iSel>=0 )
		{
		m_ctlExplaination.SetWindowText(_gszPredefinedDescriptions[iSel]);
		m_cApply.EnableWindow(TRUE);	
		}
	
}

void CPenalityDlg::OnEditchangeCodeCombo() 
{
		m_cApply.EnableWindow(TRUE);	
}


void CPenalityDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CPenalityDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();
}

void CPenalityDlg::OnColumnclickPenalityList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_cPenalityList.SortItems( ComparePenalty,pNMListView->iSubItem );
	m_iSortColumn=pNMListView->iSubItem;

	*pResult = 0;
}
