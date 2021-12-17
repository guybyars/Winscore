// DetailedScoreInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "DetailedScoreInfoDlg.h"
#include "WSClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailedScoreInfoDlg dialog


CDetailedScoreInfoDlg::CDetailedScoreInfoDlg(CWinscoreDoc *pDoc,
											 CScoreRecord *pcRecord,
											CWnd* pParent /*=NULL*/)
	: CResizingDialog(CDetailedScoreInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailedScoreInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    SetControlInfo(IDCANCEL,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_COPY,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_LISTCTRL,    RESIZE_HOR | RESIZE_VER);

	m_pcRecord=pcRecord;
	m_pDoc=pDoc;
}


void CDetailedScoreInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailedScoreInfoDlg)
	DDX_Control(pDX, IDC_LISTCTRL, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailedScoreInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailedScoreInfoDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailedScoreInfoDlg message handlers

BOOL CDetailedScoreInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CreateStatusColumns();
	CString cText;

	CContestant* pcContestant=m_pDoc->m_contestantList.GetByContestNo(m_pcRecord->m_cContestNo);


	CWSClass &cClass=GetClass(m_pcRecord->m_eClass);

	AddTwoStringItem(_T("ID"), m_pcRecord->m_cContestNo);
	AddTwoStringItem(_T("Name"), pcContestant->m_strLastName );

	AddTwoStringItem(_T("Class"), cClass.GetName() );
	AddTwoStringItem(_T("Date"),  m_pcRecord->m_cDate.Format( _T("%A, %B %d, %Y")));
	AddTwoStringItem(_T("Guest"),  m_pcRecord->m_bGuest?_T("Yes"):_T("No"));
	AddTwoStringItem(_T("Contestant"),  m_pcRecord->m_fContestant?_T("Yes"):_T("No"));
	AddTwoStringItem(_T("Finisher"),  m_pcRecord->m_fFinisher?_T("Yes"):_T("No"));
	AddTwoStringItem(_T("Status"),  m_pcRecord->StatusText());
	AddTwoStringItem(_T("Remarks"),  m_pcRecord->PenalityText());


	AddTwoStringItem(_T("Start Time"),  m_pcRecord->StartTimeText());
	AddTwoStringItem(_T("Finish Time"),  m_pcRecord->FinishTimeText());
	AddTwoStringItem(_T("TOC"),  m_pcRecord->TOCText());
	AddTwoStringItem(_T("STOC"),  m_pcRecord->STOCText());


	cText.Format(_T("%4.0lf"), m_pcRecord->m_dPoints);
	AddTwoStringItem(_T("Points"),  cText);

	cText.Format(_T("%4.0lf"), m_pcRecord->m_dCumPoints);
	AddTwoStringItem(_T("Cum Points"),  cText);

	cText.Format(_T("%4.0lf"), m_pcRecord->m_dAppliedPenalityPoints);
	AddTwoStringItem(_T("Applied Penalty"),  cText);


    cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dSpeed, SYSTEMUNITS, m_pDoc->m_eUnits) );
	AddTwoStringItem(_T("Speed"),  cText);
  
	cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dScoredSpeed, SYSTEMUNITS, m_pDoc->m_eUnits) );
	AddTwoStringItem(_T("Scored Speed"),  cText);

	if( cClass.IsHandicapped() )
		{
		cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dHandicappedSpeed, SYSTEMUNITS, m_pDoc->m_eUnits) );
		AddTwoStringItem(_T("Handicapped Speed"),  cText);
  
		cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dScoredHandicappedSpeed, SYSTEMUNITS, m_pDoc->m_eUnits) );
		AddTwoStringItem(_T("Scored Handicapped Speed"),  cText);
		}

	cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dDistance, SYSTEMUNITS, m_pDoc->m_eUnits) );
	AddTwoStringItem(_T("Distance"),  cText);

	if( cClass.IsHandicapped() )
		{
		cText.Format(_T("%6.5lf"), 	ConvertDistance(m_pcRecord->m_dHandicappedDistance, SYSTEMUNITS, m_pDoc->m_eUnits) );
		AddTwoStringItem(_T("Handicapped Distance"),  cText);
		}

	if( cClass.IsUsingWDSA() )
		{
		cText.Format(_T("%6.5lf"), 	m_pcRecord->m_dWDSA );
		AddTwoStringItem(_T("Worst Day Scoring Adjustment"),  cText);
		}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	
}

#define NUM_COLUMNS 2

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("*"), _T("Status")
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	150, 250
};

void CDetailedScoreInfoDlg::CreateStatusColumns()
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		m_cListCtrl.InsertColumn(i,&lvc);
	}	
}

void CDetailedScoreInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CDetailedScoreInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
}


void CDetailedScoreInfoDlg::AddTwoStringItem( CString cstr1, CString cstr2)
	{
	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

    int iItem=m_cListCtrl.GetItemCount();

	//  Load up the listbox
	lvi.iItem = iItem;
	m_cListCtrl.InsertItem(&lvi);
	m_cListCtrl.SetItemText(iItem,0,cstr1);
	m_cListCtrl.SetItemText(iItem,1,cstr2);
	}

void CDetailedScoreInfoDlg::OnCopy() 
{
	CString str;

	for( int i=0; i<m_cListCtrl.GetItemCount(); i++ )
		{
		str+=m_cListCtrl.GetItemText( i, 0);
		str+="\t";
		str+=m_cListCtrl.GetItemText( i, 1);
		str+="\r\n";
		}

    HGLOBAL hglobalText = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, str.GetLength() + 1);
    strcpy_s((TCHAR *)GlobalLock(hglobalText), str.GetLength() + 1, str);
    GlobalUnlock(hglobalText);
    
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,hglobalText);
    CloseClipboard();
	
}
