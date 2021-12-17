//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// InvalidLegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "InvalidLegDlg.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if 0
static BOOL	fModified=FALSE;

/////////////////////////////////////////////////////////////////////////////
// CInvalidLegDlg dialog


CInvalidLegDlg::CInvalidLegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInvalidLegDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvalidLegDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInvalidLegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvalidLegDlg)
	DDX_Control(pDX, IDC_LIST, m_cListBox);
	DDX_Control(pDX, IDC_TO_COMBO, m_cTurnpointToCombo);
	DDX_Control(pDX, IDC_FROM_COMBO, m_cTurnpointFromCombo);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_APPLY, m_cApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvalidLegDlg, CDialog)
	//{{AFX_MSG_MAP(CInvalidLegDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_FROM_COMBO, OnSelchangeFromCombo)
	ON_CBN_SELCHANGE(IDC_TO_COMBO, OnSelchangeToCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, OnKeydownList)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvalidLegDlg message handlers

void CInvalidLegDlg::OnApply() 
{

	CInvalidLeg  cInvalidLeg;

	CTurnpoint* pcFrom=m_cTurnpointFromCombo.GetCurSel();
	CTurnpoint* pcTo=m_cTurnpointToCombo.GetCurSel();

	if( pcFrom==NULL || pcTo==NULL ) return;

	cInvalidLeg.m_iFirstTurnpoint=pcFrom->m_iID;
	cInvalidLeg.m_iSecondTurnpoint=pcTo->m_iID;

	if( pcFrom == pcTo )
		{
		AfxMessageBox( SAME_INVALID_LEGS, MB_OK  );
		return;
		}

	m_pcInvalidLegList->AddToList(&cInvalidLeg);

	fModified=TRUE;
	UpdateListBox();

	m_cApply.EnableWindow(FALSE);
}

void CInvalidLegDlg::UpdateListBox()
	{
	m_pcInvalidLegList->LoadInvalidLegList(m_cListBox, m_pDoc->m_turnpointArray);
	}

void CInvalidLegDlg::OnDelete() 
{
	int iSel = m_cListBox.GetNextItem( -1, LVNI_SELECTED );
	if( iSel<0) return;
	CInvalidLeg* pcInvalidLeg=(CInvalidLeg*)m_cListBox.GetItemData(iSel);
	if(pcInvalidLeg==NULL) return;

	m_pcInvalidLegList->RemoveByPointer(pcInvalidLeg);
	m_cDelete.EnableWindow(FALSE);
	UpdateListBox();
	fModified=TRUE;
	
}

void CInvalidLegDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CInvalidLegDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
	m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cTurnpointFromCombo);
	m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cTurnpointToCombo);

	m_pcInvalidLegList->CreateControlColumns(m_cListBox);

	m_cDelete.EnableWindow(FALSE);

	UpdateListBox();
	fModified=FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CInvalidLegDlg::OnSelchangeFromCombo() 
{
	m_cApply.EnableWindow(TRUE);	
}

void CInvalidLegDlg::OnSelchangeToCombo() 
{
	m_cApply.EnableWindow(TRUE);	
}

void CInvalidLegDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ( pNMListView->uNewState & LVIS_SELECTED )
		{ 
		m_cDelete.EnableWindow(TRUE);
		}	
	*pResult = 0;
}

void CInvalidLegDlg::OnCancel() 
{
	if( fModified ) 
		{		
		if( AfxMessageBox( LOST_LEGS,  MB_OKCANCEL )==2 )return;
		}
	CDialog::OnCancel();
}

void CInvalidLegDlg::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if( pLVKeyDow->wVKey==DELETE_KEY ) 
		{
		OnDelete();
		}	
	*pResult = 0;
}

#define MAXLINELEN	80

void CInvalidLegDlg::OnImport() 
{

	int		nAdded=0;
	CString  cFileName="";

	static char BASED_CODE szFilter[] = "Turnpoint Exchange LGS file (*.lgs)|*.lgs|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	//static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog  cFileDlg(TRUE, "txt", cFileName, OFN_HIDEREADONLY, szFilter);
	cFileDlg.m_ofn.lpstrTitle="Import Invalid Legs";

	int iOriginalCount=m_pcInvalidLegList->GetCount();

	if( cFileDlg.DoModal()==IDOK )
		{

		char  cRecord[MAXLINELEN];
		int nLinesRead=0;
		ifstream	cIfstream;

		try
		{
		//  Use the ifstream class to do I/O on the ascii files.
		cIfstream.open( cFileDlg.GetPathName() );
		if (cIfstream.is_open()==0)
			{
			return;
			}

		while( !cIfstream.eof() )
			{
			cIfstream.getline( cRecord, MAXLINELEN );
			nLinesRead++;
			if( cIfstream.fail()  ) break;

			int iFirst, iSecond;
			int iRead=sscanf_s(cRecord, "%d%d", &iFirst, &iSecond);
			if( iRead!=2 ) 
				{
				CString cError;
				cError.Format("Error reading line %d, couldn't find two turnpoint ID's", nLinesRead);
				AfxMessageBox(cError);
				break;
				}

			if( iFirst==iSecond ) continue;

			CInvalidLeg  cInvalidLeg;

			CTurnpoint* pcFrom=m_pDoc->m_turnpointArray[iFirst-1];
			CTurnpoint* pcTo=m_pDoc->m_turnpointArray[iSecond-1];

			if( pcFrom==NULL || pcTo==NULL ) continue;

			cInvalidLeg.m_iFirstTurnpoint=pcFrom->m_iID;
			cInvalidLeg.m_iSecondTurnpoint=pcTo->m_iID;

			if( pcFrom == pcTo )
				{
	//			AfxMessageBox( SAME_INVALID_LEGS, MB_OK  );
				continue;
				}

			if( !m_pcInvalidLegList->IsLegAlreadyInList(cInvalidLeg) ) 
				{
				m_pcInvalidLegList->AddToList(&cInvalidLeg);
				nAdded++;
				}
			if( cIfstream.eof() ) break;

			}

//			ifstream ifs( cFileDlg.GetPathName() );
//			ifs >> *m_pcInvalidLegList;
//			ifs.close();
			}
		catch (CException* pex)
			{
			char cError[256];
			pex->GetErrorMessage(cError, 256);
			AfxMessageBox("Error Reading Invalid Leg File: "+cFileDlg.GetPathName(), MB_ICONSTOP );
			return;
			}
		UpdateListBox();
		UpdateData(FALSE);
		fModified=1;
		CString cStatus;
	
		if( iOriginalCount<m_pcInvalidLegList->GetCount() )
			{
			cStatus.Format("%d Invalid Legs successfully Imported from: %s", m_pcInvalidLegList->GetCount()-iOriginalCount, cFileDlg.GetPathName());
			AfxMessageBox(  cStatus,	MB_OK | MB_ICONINFORMATION, 0 );
			}
		}		
	
}

#endif