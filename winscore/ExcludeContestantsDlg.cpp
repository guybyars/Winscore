//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ExcludeContestantsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ExcludeContestantsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define NUM_COLUMNS 6

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
   _T("Include"),	_T("Name"), _T("ID"), _T("Glider"), _T("Class"),  _T("Citizenship"), 
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,	LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	50, 150, 50, 80, 70, 70 };




/////////////////////////////////////////////////////////////////////////////
// CExcludeContestantsDlg dialog


CExcludeContestantsDlg::CExcludeContestantsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcludeContestantsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcludeContestantsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExcludeContestantsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcludeContestantsDlg)
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExcludeContestantsDlg, CDialog)
	//{{AFX_MSG_MAP(CExcludeContestantsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcludeContestantsDlg message handlers

BOOL CExcludeContestantsDlg::OnInitDialog() 
{

	LV_COLUMN lvc;
	LV_ITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM ;
	lvi.iItem = 0;
	int iItem=0;

	CDialog::OnInitDialog();

	m_cListCtrl.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		m_cListCtrl.InsertColumn(i,&lvc);
	}
	LoadPilotListBox();


	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CExcludeContestantsDlg::LoadPilotListBox()
{
	LV_ITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM ;
	lvi.iItem = 0;
	int iItem=0;

	m_cListCtrl.DeleteAllItems();

	POSITION pos = m_pDoc->m_contestantList.GetHeadPosition();
	while( pos!=NULL)
		{
		CContestant* pcContestant= m_pDoc->m_contestantList.GetNext(pos);
		if( pcContestant )
			{
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcContestant;
			m_cListCtrl.InsertItem(&lvi);

			m_cListCtrl.SetItemText(iItem,0,_T(""));
			m_cListCtrl.SetItemText(iItem,1,pcContestant->NameText());
			m_cListCtrl.SetItemText(iItem,2,pcContestant->m_strContestNo);
			m_cListCtrl.SetItemText(iItem,3,pcContestant->m_strGlider);
			m_cListCtrl.SetItemText(iItem,4,GetClass(pcContestant->m_eClass).GetName());
			m_cListCtrl.SetItemText(iItem,5,pcContestant->CitizenText());
			m_cListCtrl.SetItemData(iItem, (LPARAM)pcContestant);

			BOOL b=pcContestant->IsExcludedFromFlightFiles();

 			ListView_SetCheckState(m_cListCtrl.m_hWnd, iItem, !b);

			iItem++;
			}
		}
}
void CExcludeContestantsDlg::OnOK() 
{
	CContestant* pcContestant;
	int iItem= -1;
	while(1)
		{
		iItem=m_cListCtrl.GetNextItem( iItem, LVNI_ALL   );
		if( iItem<0 ) break;
		pcContestant=(CContestant*)m_cListCtrl.GetItemData(iItem);
		if( !pcContestant ) continue;

		pcContestant->ExcludeFromFlightFiles( !ListView_GetCheckState( m_cListCtrl.m_hWnd,  iItem) );
		}
	
	CDialog::OnOK();
}

