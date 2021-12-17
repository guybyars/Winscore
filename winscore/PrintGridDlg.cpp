//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PrintGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "PrintGridDlg.h"
#include "classlistctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintGridDlg dialog


CPrintGridDlg::CPrintGridDlg(CWinscoreDoc* pDoc, CGridData* pGridData, CWnd* pParent /*=NULL*/)
	: CDialog(CPrintGridDlg::IDD, pParent),
	m_pDoc(pDoc),
	m_pGridData(pGridData)
{

	//{{AFX_DATA_INIT(CPrintGridDlg)
	m_iStartNumber = 1;
	m_iNumRows = 1;
	m_iSpaces = 0;
	m_iNumDaysToPrint = 0;
	m_bLetterCheck = FALSE;
	//}}AFX_DATA_INIT

}


void CPrintGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintGridDlg)
	DDX_Control(pDX, IDC_START_SPIN, m_cStartSpin);
	DDX_Control(pDX, IDC_SPACES_SPIN, m_cSpacesSpin);
	DDX_Control(pDX, IDC_DAYS_TO_PRINT_SPIN, m_cDaysToPrintSpin);
	DDX_Control(pDX, IDC_SPACES_TEXT, m_cSpacesText);
	DDX_Control(pDX, IDC_SPACES, m_cSpacesEdit);
	DDX_Control(pDX, IDC_NUM_ROWS_EDIT, m_cNumRowsEdit);
	DDX_Control(pDX, IDC_NUM_ROWS_SPIN, m_cRowsSpin);
	DDX_Control(pDX, IDC_CLASS_LIST, m_cClassListCtrl);
	DDX_Text(pDX, IDC_START_GRID_NUMBER, m_iStartNumber);
	DDX_Text(pDX, IDC_NUM_ROWS_EDIT, m_iNumRows);
	DDV_MinMaxInt(pDX, m_iNumRows, 1, 20);
	DDX_Text(pDX, IDC_SPACES, m_iSpaces);
	DDV_MinMaxInt(pDX, m_iSpaces, 0, 1000);
	DDX_Text(pDX, IDC_DAYS_TO_PRINT, m_iNumDaysToPrint);
	DDV_MinMaxInt(pDX, m_iNumDaysToPrint, 1, 10);
	DDX_Check(pDX, IDC_LETTER_CHECK, m_bLetterCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintGridDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintGridDlg)
	ON_BN_CLICKED(IDC_DAY_PLUS, OnDayPlus)
	ON_BN_CLICKED(IDC_DAY_MINUS, OnDayMinus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NUM_ROWS_SPIN, OnDeltaposNumRowsSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintGridDlg message handlers

void CPrintGridDlg::OnOK() 
{
	if( !UpdateData(TRUE) ) return;

	int m_nClassesToPrint=m_cClassListCtrl.GetNumCheckedClasses();

	if( m_nClassesToPrint==0 ) 
		{
		AfxMessageBox(_T("No classes were selected."));
		return;
		}

	for( int i=0; i<m_nClassesToPrint; i++ )
		{
		m_pGridData->aiPrintClasses[i]	=m_cClassListCtrl.GetCheckedClass(i);
		m_pGridData->aiPrintDays[i]		=m_cClassListCtrl.GetDay(m_pGridData->aiPrintClasses[i]);
		m_pDoc->CalculateGridPositions(	m_pGridData->aiPrintClasses[i], false);
		}

	m_pGridData->iNumDaysToPrint	=m_iNumDaysToPrint;
	m_pGridData->iPrintStartNumber	=m_iStartNumber;
	m_pGridData->iPrintNumRows		=m_iNumRows;
	m_pGridData->iPrintNumClasses	=m_nClassesToPrint;
	m_pGridData->bLetterScheme		=(m_bLetterCheck)?(true):(false);
	m_pGridData->iSpaces			=m_iSpaces;

	CDialog::OnOK();
}


BOOL CPrintGridDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_iNumDaysToPrint	=m_pGridData->iNumDaysToPrint;
	m_iStartNumber		=m_pGridData->iPrintStartNumber;
	m_iNumRows			=m_pGridData->iPrintNumRows;
	m_bLetterCheck		=m_pGridData->bLetterScheme;
	m_iSpaces			=m_pGridData->iSpaces;

	m_cClassListCtrl.SetDayDisplay(true);
	m_cClassListCtrl.AddColumn(_T("Class"));	
	m_cClassListCtrl.SetAvailableClasses();
	
	int nClasses=m_cClassListCtrl.GetItemCount();
	m_cSpacesEdit.EnableWindow(nClasses>1);
	m_cSpacesText.EnableWindow(nClasses>1);

	m_cRowsSpin.SetRange( 1, 10 );
	m_cDaysToPrintSpin.SetRange( 1, 20 );
	m_cSpacesSpin.SetRange( 0, 50 );
	m_cStartSpin.SetRange( 1, 1000 );

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintGridDlg::OnDayPlus() 
{
	m_cClassListCtrl.IncrementSelectedDay(true);	
}

void CPrintGridDlg::OnDayMinus() 
{
	m_cClassListCtrl.IncrementSelectedDay(false);		
}

void CPrintGridDlg::OnDeltaposNumRowsSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	*pResult = 0;
}

CGridData::CGridData()
	{
	bLetterScheme		=false;
	iNumDaysToPrint		=1;
	iPrintStartNumber	=1;
	iPrintNumRows		=4;
	iPrintNumClasses	=0;
	iSpaces				=0;
	};

