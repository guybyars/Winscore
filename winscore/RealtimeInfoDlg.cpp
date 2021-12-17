//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// RealtimeInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "fix.h"
#include "fixlist.h"
#include "RealtimeInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool	bSortDirection=false;
static int  iLastSorted=0;

/////////////////////////////////////////////////////////////////////////////
// CRealtimeInfoDlg dialog


CRealtimeInfoDlg::CRealtimeInfoDlg(CFixList &cFixList, CString strContestNo, CWnd* pParent /*=NULL*/)
	: CResizingDialog(CRealtimeInfoDlg::IDD, pParent),
	m_cFixList(cFixList),
	m_strContestNo(strContestNo)
{
	//{{AFX_DATA_INIT(CRealtimeInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    SetControlInfo(IDC_WSVIEWCTRL1,		RESIZE_HOR | RESIZE_VER);


}


void CRealtimeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealtimeInfoDlg)
	DDX_Control(pDX, IDC_LIST, m_cList);
	//}}AFX_DATA_MAP

    SetControlInfo(IDC_LIST,		RESIZE_HOR | RESIZE_VER);

}


BEGIN_MESSAGE_MAP(CRealtimeInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CRealtimeInfoDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnclickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealtimeInfoDlg message handlers


void CRealtimeInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);	
}

void CRealtimeInfoDlg::OnPaint() 
{
	 CPaintDC dc(this); // device context for painting		
	 CResizingDialog::OnPaint();
}

#define NUM_COLUMNS 4

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
	{
	_T("Time"), 
	_T("Location"), 
	_T("Change in Location"), 
	_T("Altitude")
	};


static int _gnColumnWidth[NUM_COLUMNS] = 
{
	80, //time
	195, //location
	170, //location
	75 //altitude
};

void CRealtimeInfoDlg::CreateControlColumns(CListCtrl& ListCtrl)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i; 
		CString str=_gszColumnLabel[i];

		lvc.pszText = (char*)LPCTSTR(str);
		lvc.cx =  _gnColumnWidth[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

}



BOOL CRealtimeInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CreateControlColumns(m_cList);

	CFix* pcPrevFix=NULL;

	int iItem=0;
	double dDist=0.;
	POSITION pos = m_cFixList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cFixList.GetNext(pos);
		if( !pcFix ) continue;

		if( pcFix->m_strContestNo!=m_strContestNo ) continue;
	
		CLocation cLoc( pcFix->m_fLatitude, pcFix->m_fLongitude);
		if( pcPrevFix )
			{
			CLocation cPrevLoc( pcPrevFix->m_fLatitude, pcPrevFix->m_fLongitude);
			dDist=cPrevLoc.DistanceTo(&cLoc, eStatute);
			}
		else
			dDist=0.0;

		LV_ITEM lvi;

		lvi.iSubItem = 0;
		lvi.mask = 0;
		int i=0;

		lvi.iItem=iItem++; 
		int iItem=m_cList.InsertItem(&lvi);
		CString cInt;
		CString cDist;
		cInt.Format("%d", pcFix->m_iAltitude );
		cDist.Format("%5.2f Mi",dDist);
		m_cList.SetItemText(iItem,i++, pcFix->m_cTime.Format( "%H:%M:%S"));
		m_cList.SetItemText(iItem,i++, cLoc.GetFormattedInfo() );
		m_cList.SetItemText(iItem,i++, cDist );
		m_cList.SetItemText(iItem,i++, cInt);

		pcPrevFix=pcFix;

	}

	SetWindowText( "Fix info. for: "+ m_strContestNo);
	
	m_cList.SortTextItems( 0, false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealtimeInfoDlg::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if( iLastSorted==pNMListView->iSubItem ) 
		bSortDirection=!bSortDirection;
	else
		bSortDirection=false;

	m_cList.SortTextItems( pNMListView->iSubItem, bSortDirection);

	iLastSorted=pNMListView->iSubItem;
	
	*pResult = 0;
}

