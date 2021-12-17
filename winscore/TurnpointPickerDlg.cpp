//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointPickerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "TurnpointPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnpointPickerDlg dialog


CTurnpointPickerDlg::CTurnpointPickerDlg(CTurnpointArray &turnPointArray, CWnd* pParent /*=NULL*/)
	: CDialog(CTurnpointPickerDlg::IDD, pParent),
	m_turnPointArray(turnPointArray),
	m_pSelectedTurnpoint(NULL)
{
	//{{AFX_DATA_INIT(CTurnpointPickerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTurnpointPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurnpointPickerDlg)
	DDX_Control(pDX, IDC_LIST, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTurnpointPickerDlg, CDialog)
	//{{AFX_MSG_MAP(CTurnpointPickerDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnpointPickerDlg message handlers

void CTurnpointPickerDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int nItem= m_cListCtrl.GetNextItem( 0, LVNI_FOCUSED );

	if( nItem<0 ) 
		{
		CDialog::OnCancel();
		}
	else
		{
		m_pSelectedTurnpoint=(CTurnpoint*)m_cListCtrl.GetItemData(nItem);
	   	CDialog::OnOK();
		}	
	
	*pResult = 0;
}


BOOL CTurnpointPickerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	m_turnPointArray.CreateControlColumns( m_cListCtrl, eHHMMMM);
	m_turnPointArray.LoadTurnpointList( m_cListCtrl, eHHMMMM, true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTurnpointPickerDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;
}

