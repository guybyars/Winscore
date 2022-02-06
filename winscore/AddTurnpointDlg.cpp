//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// AddTurnpointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "Location.h"
#include "Turnpoint.h"
#include "WinscoreDoc.h"
#include "ResizingDialog.h"
#include "AddTurnpointDlg.h"


#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static  bool bDeleteWarned=false;
static  int fModified;
static  int iSortColumn=0;
static	ELatDirection eLatDirection=eNORTH;
static 	ELongDirection eLongDirection=eWEST;

/////////////////////////////////////////////////////////////////////////////
// CAddTurnpointDlg dialog


CAddTurnpointDlg::CAddTurnpointDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CAddTurnpointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddTurnpointDlg)
	m_iLatDeg = 39;
	m_iLongDeg = 84;
	m_iLongSec = 23;
	m_Name = _T("HomeAirport");
	m_dLatMin = 38.;
	m_dLongMin = 5.;
	m_iLatSec = 31;
	m_iElevation = 0;
	m_strID = _T("");
	//}}AFX_DATA_INIT
	m_pPreselect=NULL;

    SetControlInfo(IDOK,				ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,			ANCHORE_BOTTOM);
    SetControlInfo(IDDELETE,			ANCHORE_BOTTOM);
    SetControlInfo(IDAPPLY,				ANCHORE_BOTTOM);
    SetControlInfo(IDC_IMPORT,          ANCHORE_BOTTOM);
    SetControlInfo(IDC_LIST_CONTROL,    RESIZE_HOR | RESIZE_VER);

}


void CAddTurnpointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddTurnpointDlg)
	DDX_Control(pDX, IDC_TURNPOINT_CHECK, m_cTurnpointCheck);
	DDX_Control(pDX, IDC_START_CHECK, m_cStartCheck);
	DDX_Control(pDX, IDC_AIRFIELD_CHECK, m_cAirfieldCheck);
	DDX_Control(pDX, IDC_HOME_CHECK, m_cHomeCheck);
	DDX_Control(pDX, IDC_FINISH_CHECK, m_cFinishCheck);
	DDX_Control(pDX, IDC_IMPORT, m_cImport);
	DDX_Control(pDX, IDC_LIST_CONTROL, m_cTurnpointListControl);
	DDX_Control(pDX, IDDELETE, m_CDelete);
	DDX_Control(pDX, IDC_LONG_SEC_TEXT, m_cLongSecText);
	DDX_Control(pDX, IDC_LAT_SEC_TEXT, m_cLatSecText);
	DDX_Control(pDX, IDC_NAME, m_CName);
	DDX_Control(pDX, IDC_LONG_SEC, m_CLongSec);
	DDX_Control(pDX, IDC_LAT_SEC, m_CLatSec);
	DDX_Control(pDX, IDAPPLY, m_CApply);
	DDX_Control(pDX, IDC_TURNPOINT_ID, m_CID);
	DDX_Control(pDX, IDC_SOUTH, m_CSouth);
	DDX_Control(pDX, IDC_WEST, m_CWest);
	DDX_Control(pDX, IDC_NORTH, m_CNorth);
	DDX_Control(pDX, IDC_EAST, m_CEast);
	DDX_Text(pDX, IDC_LAT_DEG, m_iLatDeg);
	DDV_MinMaxInt(pDX, m_iLatDeg, 0, 180);
	DDX_Text(pDX, IDC_LONG_DEG, m_iLongDeg);
	DDV_MinMaxInt(pDX, m_iLongDeg, 0, 180);
	DDX_Text(pDX, IDC_LONG_SEC, m_iLongSec);
	DDV_MinMaxInt(pDX, m_iLongSec, 0, 60);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 25);
	DDX_Text(pDX, IDC_LAT_MIN, m_dLatMin);
	DDV_MinMaxDouble(pDX, m_dLatMin, 0., 60.);
	DDX_Text(pDX, IDC_LONG_MIN, m_dLongMin);
	DDV_MinMaxDouble(pDX, m_dLongMin, 0., 60.);
	DDX_Text(pDX, IDC_LAT_SEC, m_iLatSec);
	DDV_MinMaxInt(pDX, m_iLatSec, 0, 60);
	DDX_Text(pDX, IDC_ELEVATION, m_iElevation);
	DDV_MinMaxInt(pDX, m_iElevation, 0, 30000);
	DDX_Text(pDX, IDC_TURNPOINT_ID, m_strID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddTurnpointDlg, CDialog)
	//{{AFX_MSG_MAP(CAddTurnpointDlg)
	ON_BN_CLICKED(IDC_WEST, OnWest)
	ON_BN_CLICKED(IDC_SOUTH, OnSouth)
	ON_BN_CLICKED(IDC_NORTH, OnNorth)
	ON_BN_CLICKED(IDC_EAST, OnEast)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_KILLFOCUS(IDC_LAT_MIN, OnKillfocusLatMin)
	ON_EN_KILLFOCUS(IDC_LONG_MIN, OnKillfocusLongMin)
	ON_EN_KILLFOCUS(IDC_TURNPOINT_ID, OnKillfocusTurnpointId)
	ON_BN_CLICKED(IDDELETE, OnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONTROL, OnItemchangedListControl)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CONTROL, OnColumnclickListControl)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_CONTROL, OnItemclickListControl)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CONTROL, OnKeydownListControl)
	ON_EN_KILLFOCUS(IDC_NAME, OnKillfocusName)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_TURNPOINT_ID, OnChangeTurnpointId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddTurnpointDlg message handlers


BOOL CAddTurnpointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

 	m_pturnpointArray->CreateControlColumns(m_cTurnpointListControl, m_eCoordinateFormat);

    CTurnpoint* pcTurnpointItem = m_pturnpointArray->GetAt(0);
    if( pcTurnpointItem ) 
		{
		UpdateFormFromTurnpoint(pcTurnpointItem);
		m_CApply.SetWindowText(_T("Apply"));
		m_CApply.EnableWindow(FALSE);
		m_cTurnpointListControl.Select(0);
		}
	else
		{
		m_strID.Format(_T("%d"),NextAvailable());
		m_CApply.SetWindowText(_T("New"));
		m_CApply.EnableWindow(TRUE);
		}

	UpdateHMSButtons();
	UpdateDirectionButtons();
    UpdateListBox();

//  If there are no tasks defined, Allow import of turnpoints.
///	m_cImport.EnableWindow(!m_bTasks  );

	fModified=0;
	iSortColumn=0;
	UpdateData(FALSE);

	if( m_pPreselect )
		{
		UpdateFormFromTurnpoint( m_pPreselect );
		UpdateDirectionButtons();
		UpdateData(FALSE);
		}

	int iID=GetID();
	for( int iItem=0; iItem<m_cTurnpointListControl.GetItemCount(); iItem++ )
			{
			pcTurnpointItem=(CTurnpoint*)m_cTurnpointListControl.GetItemData(iItem);
			if( pcTurnpointItem && pcTurnpointItem->m_iID==iID )
				{
				m_cTurnpointListControl.Select(iItem);
				m_cTurnpointListControl.EnsureVisible(iItem, true);
				break;
				}
			}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void CAddTurnpointDlg::OnWest() 
{
	eLongDirection=eWEST;
	UpdateDirectionButtons();
}

void CAddTurnpointDlg::OnSouth() 
{
    eLatDirection=eSOUTH;
	UpdateDirectionButtons();
}

void CAddTurnpointDlg::OnNorth() 
{
    eLatDirection=eNORTH;
    UpdateDirectionButtons();
}

void CAddTurnpointDlg::OnEast() 
{
    eLongDirection=eEAST;
    UpdateDirectionButtons();
}



void CAddTurnpointDlg::OnApply() 
{	 

	if( UpdateData()==0 ) return;
   
    m_Name.TrimRight();
    m_Name.TrimLeft();
	CLocation* pcLocationItem;

	int iID=GetID();

	if( iID<=0 ) 
		{
		AfxMessageBox(_T("Invalid control point ID"));
		return;
		}


    if( iID<=0 || iID>MAX_TURNPOINTS )
	{
	 AfxMessageBox( IDS_ERR_MAX_TURNPOINTS,  MB_OK );
	  return;
	};

   if( m_Name==_T("") )
	{
	 AfxMessageBox( IDS_ERR_NO_TURNPOINT_NAME,  MB_OK );
	  return;
	};

    int jID=FindName(m_Name);
    if( jID>0 && jID!=iID )
	{
	 AfxMessageBox( IDS_ERR_DUPLICATE_TURNPOINT_NAME,  MB_OK );
	 return;
	}

    if(  m_eCoordinateFormat==eHHMMSS )
		{
		 pcLocationItem=new CLocation(m_iLatDeg, m_dLatMin, m_iLatSec,
									m_iLongDeg, m_dLongMin, m_iLongSec,
									eLatDirection, eLongDirection);
		}
	else
		{	
		pcLocationItem =new CLocation(m_iLatDeg, m_dLatMin,
									m_iLongDeg, m_dLongMin,
									eLatDirection, eLongDirection);
		}

	int iAttribute=0;
	if( m_cHomeCheck.GetCheck() ) iAttribute |=TPT_HOME;
	if( m_cStartCheck.GetCheck() ) iAttribute |=TPT_START;
	if( m_cFinishCheck.GetCheck() ) iAttribute |=TPT_FINISH;
	if( m_cAirfieldCheck.GetCheck() ) iAttribute |=TPT_AIRFIELD;
	if( m_cTurnpointCheck.GetCheck() ) iAttribute |=TPT_CONTROL;

	CTurnpoint* pTurnpoint = NewTurnpoint(iID,m_Name,pcLocationItem, iAttribute, m_iElevation);
    delete pcLocationItem;


	bool bUpdateAll=TRUE;
	for( int i=0; i<m_cTurnpointListControl.GetItemCount(); i++ )
		{
		if( _ttoi(m_cTurnpointListControl.GetItemText(i,0))==iID )
			{
			pTurnpoint->LoadListItem(	m_cTurnpointListControl, 
										i, 
										m_eCoordinateFormat);
			bUpdateAll=FALSE;
			break;
			}
		}

	if( bUpdateAll ) 
		{
		UpdateListBox();

		int iSel = m_cTurnpointListControl.GetNextItem( -1, LVNI_SELECTED );
		m_cTurnpointListControl.EnsureVisible( iID-1, FALSE );

		if( iSel<0 )
			{
			if( iID<MAX_TURNPOINTS ) 
						m_strID.Format(_T("%d"),NextAvailable());
			m_Name=_T("");
			UpdateData(FALSE);
			m_CName.SetFocus();
			m_CApply.EnableWindow(FALSE);
			}
		}
	
	UpdateData(FALSE);
	fModified=1;

}

void CAddTurnpointDlg::UpdateListBox() 
	{
 	m_pturnpointArray->LoadTurnpointList( m_cTurnpointListControl, m_eCoordinateFormat );
	m_cTurnpointListControl.SortItems( CompareTurnpoint, iSortColumn ); 
	}


void CAddTurnpointDlg::SetTurnpointArray(CTurnpointArray* pcTurnpointArray)
{
	m_pturnpointArray=pcTurnpointArray;
}



CTurnpoint* CAddTurnpointDlg::NewTurnpoint(int iID, 
										   CString strName, 
										   CLocation* pcLocation,
										   int			iAttribute,
										   int			iElevation)
	{

	CTurnpoint* pTurnpointItem = new CTurnpoint(iID, strName, pcLocation, iAttribute, iElevation);
	m_pturnpointArray->RemoveAt(GetID()-1);
    m_pturnpointArray->SetAt(iID-1, pTurnpointItem);
	
	return pTurnpointItem;
	}



int CAddTurnpointDlg::NextAvailable()
{	
  CTurnpoint* pTurnpointItem;
  for ( int i=0; i<MAX_TURNPOINTS; i++)
	{
    pTurnpointItem = m_pturnpointArray->GetAt(i);
    if( !pTurnpointItem ) return(i+1);
	}
  return 1;
}



int CAddTurnpointDlg::FindName(CString strName)
{
	CTurnpoint* pTurnpointItem;

  for ( int i=0; i<MAX_TURNPOINTS; i++)
	{
    pTurnpointItem = m_pturnpointArray->GetAt(i);
    if( pTurnpointItem && 
		(pTurnpointItem->m_strName==strName) ) return(i+1);
	}

return(-1);
}

void CAddTurnpointDlg::UpdateFormFromTurnpoint(CTurnpoint* pcTurnpointItem)
	{

	if( pcTurnpointItem==NULL)
		{
//		m_strID="";
		m_Name="";
		m_cHomeCheck.SetCheck(0);
		m_cStartCheck.SetCheck(0);
		m_cFinishCheck.SetCheck(0);
		m_cAirfieldCheck.SetCheck(1);
		m_cTurnpointCheck.SetCheck(1);
		m_iElevation=0;
		m_CApply.SetWindowText(_T("New"));
		}
	else
		{
		m_strID.Format(_T("%d"), pcTurnpointItem->m_iID);
		m_Name=pcTurnpointItem->m_strName;

		m_iLatDeg=pcTurnpointItem->m_iLatDegrees;
		m_iLatSec=pcTurnpointItem->GetLatSeconds();

		m_iLongDeg=pcTurnpointItem->m_iLongDegrees;
		m_iLongSec=pcTurnpointItem->GetLongSeconds();

		eLatDirection=pcTurnpointItem->m_eLatDirection;
		eLongDirection=pcTurnpointItem->m_eLongDirection;

		if( m_eCoordinateFormat==eHHMMMM )
			{
			m_dLongMin=pcTurnpointItem->m_dLongMinutes;
			m_dLatMin=pcTurnpointItem->m_dLatMinutes;
			}
		else
			{
			m_dLongMin=pcTurnpointItem->GetLongMinutes();
			m_dLatMin=pcTurnpointItem->GetLatMinutes();
			}

		m_cHomeCheck.SetCheck(pcTurnpointItem->m_iAttribute & TPT_HOME );
		m_cStartCheck.SetCheck( pcTurnpointItem->m_iAttribute & TPT_START );
		m_cFinishCheck.SetCheck( pcTurnpointItem->m_iAttribute & TPT_FINISH );
		m_cAirfieldCheck.SetCheck( pcTurnpointItem->m_iAttribute & TPT_AIRFIELD );
		m_cTurnpointCheck.SetCheck( pcTurnpointItem->m_iAttribute & TPT_CONTROL );
		m_iElevation= pcTurnpointItem->m_iElevation;

		m_CApply.SetWindowText(_T("Apply"));
		m_CApply.EnableWindow(TRUE);
		}
	}


//void CAddTurnpointDlg::OnSelchangeTurnpointListbox() 
//{
	//  Update Dialogue entries based on the current selection
	//  in the list box
//	if( !m_bTasks ) m_CDelete.EnableWindow(TRUE);
//	UpdateFormFromListbox();
// }


void CAddTurnpointDlg::UpdateDirectionButtons()
{
	if( eLatDirection==eNORTH )
		{
	    m_CNorth.SetCheck(1);
		m_CSouth.SetCheck(0);
		}
	else
		{	    
		m_CNorth.SetCheck(0);
		m_CSouth.SetCheck(1);
		}

	if( eLongDirection==eEAST )
		{
	    m_CEast.SetCheck(1);
		m_CWest.SetCheck(0);
		}
	else
		{	    
		m_CEast.SetCheck(0);
		m_CWest.SetCheck(1);
		}
}

void CAddTurnpointDlg::OnKillfocusLatMin() 
{
	//  Changing minutes box, truncate if hhmmss format
	if( m_eCoordinateFormat==eHHMMSS )
		{
		UpdateData();
        modf( m_dLatMin, &m_dLatMin );
		UpdateData(FALSE);
		}	
}


void CAddTurnpointDlg::OnKillfocusLongMin() 
{

   if( m_eCoordinateFormat==eHHMMSS )
		{
		UpdateData();
        modf( m_dLongMin, &m_dLongMin );
		UpdateData(FALSE);
		}	
}

void CAddTurnpointDlg::OnKillfocusTurnpointId() 
	{
	//  Departing ID edit box, if entered number is valid,
	//  update the form with it's info
	UpdateData();	
    CTurnpoint* pcTurnpointItem = m_pturnpointArray->GetAt(GetID()-1);
    if( pcTurnpointItem )
		{
		UpdateFormFromTurnpoint(pcTurnpointItem);
		}
//	m_CTurnpointListBox.SetCurSel(-1);
	UpdateData(FALSE);	
	}



void CAddTurnpointDlg::UpdateHMSButtons() 
{

		if( m_eCoordinateFormat==eHHMMSS )
			{
			m_CLatSec.EnableWindow(TRUE);
			m_CLongSec.EnableWindow(TRUE);
			m_cLatSecText.EnableWindow(TRUE);
			m_cLongSecText.EnableWindow(TRUE);
			modf( m_dLongMin, &m_dLongMin );
			modf( m_dLatMin, &m_dLatMin );
			}
		else
			{
			m_dLongMin+=m_iLongSec/60.;
			m_dLatMin+=m_iLatSec/60.;
			m_CLatSec.EnableWindow(FALSE);
			m_CLongSec.EnableWindow(FALSE);
			m_cLatSecText.EnableWindow(FALSE);
			m_cLongSecText.EnableWindow(FALSE);
			}
		UpdateFormFromListbox();
		UpdateListBox();
		}


void	CAddTurnpointDlg::UpdateFormFromListbox()
{

//  Find the current list box selection and update the
//  form info from it

	int iSel = m_cTurnpointListControl.GetNextItem( -1, LVNI_SELECTED );
	if( iSel <0 ) return;

    CTurnpoint* pcTurnpointItem=(CTurnpoint*)m_cTurnpointListControl.GetItemData(iSel);	
	if( pcTurnpointItem ) UpdateFormFromTurnpoint(pcTurnpointItem);

	UpdateDirectionButtons();
	UpdateData(FALSE);
	return;
}

void CAddTurnpointDlg::OnDelete() 
{

	if( m_bTasks && !bDeleteWarned)
		{
		if( AfxMessageBox( _T("Deleting turnpoints after defining tasks could cause BIG TROUBLE!!!\n Proceed at own risk!."),
							MB_OKCANCEL )!=IDOK ) return;
		bDeleteWarned=true;
		}


//int iSel=m_CTurnpointListBox.GetCurSel();
 int iSel =m_cTurnpointListControl.GetNextItem( -1, LVNI_SELECTED );
 if( iSel >=0 )
	{
	CTurnpoint* pcTurnpoint=(CTurnpoint*)m_cTurnpointListControl.GetItemData(iSel);
	if( pcTurnpoint )
		{
		m_pturnpointArray->RemoveAt(pcTurnpoint->m_iID-1);
		m_cTurnpointListControl.DeleteItem(iSel);
		m_cTurnpointListControl.SetItemState((iSel-1>=0)?(iSel-1):(0), LVIS_SELECTED, LVIS_SELECTED);
		m_cTurnpointListControl.SetItemState((iSel-1>=0)?(iSel-1):(0), LVIS_FOCUSED, LVIS_FOCUSED);
		m_cTurnpointListControl.Update((iSel-1>=0)?(iSel-1):(0));
		
		UpdateData(FALSE);
		UpdateFormFromListbox();
//		UpdateListBox();
		fModified=1;
		}

	if( m_pturnpointArray->GetValidTpts()==0 )
		{ 
		m_CApply.SetWindowText(_T("New"));
		m_CDelete.EnableWindow(FALSE);
		}
	}
}

void CAddTurnpointDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if( fModified )
		{	
		int iAnswer=AfxMessageBox( _T("Save changes to Control Points?"), MB_YESNOCANCEL  );
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


void CAddTurnpointDlg::OnItemchangedListControl(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ( pNMListView->uNewState & LVIS_SELECTED )
		{ 
		UpdateFormFromListbox();
		}


	*pResult = 0;
}

void CAddTurnpointDlg::OnColumnclickListControl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_cTurnpointListControl.SortItems( CompareTurnpoint,pNMListView->iSubItem );
	*pResult = 0;
}

void CAddTurnpointDlg::OnItemclickListControl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CAddTurnpointDlg::OnImport() 
	{
	CString  cFileName="";

	if( m_pturnpointArray->GetValidTpts()>0 )
		if( AfxMessageBox( _T("Imported points may replace turnpoints already entered."), MB_OKCANCEL )!=IDOK ) return;

	if( m_bTasks )
		if( AfxMessageBox( _T("You have already defined one or more tasks.  Changing the control point list could cause BIG TROUBLE!!!\n\n Proceed at own risk?."),
							MB_OKCANCEL )!=IDOK ) return;



	static TCHAR BASED_CODE szFilter[] = _T("Cambridge Turnpoint Files (*.dat)|*.dat|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("dat"), cFileName, OFN_HIDEREADONLY, szFilter);

	cFileDlg.m_ofn.lpstrTitle=_T("Import Turnpoints From File");

	if( cFileDlg.DoModal()==IDOK )
		{
		cFileName=cFileDlg.GetPathName();

		int nImported=m_pturnpointArray->ImportTurnpoints(	cFileName, 
															cFileDlg.m_ofn.nFilterIndex);
		CString cError;
		if( nImported==0 ) 
			{
			cError=_T("Could not import control points from specified file.");
			AfxMessageBox(  cError,	MB_OK, 0 );
			}
		else
			{
			//  
			UpdateListBox();
			cError.Format(_T("%d points successfully Imported."), nImported );
			AfxMessageBox(  cError,	MB_OK | MB_ICONINFORMATION, 0 );
			UpdateFormFromTurnpoint(NULL);
			UpdateData(FALSE);
			fModified=1;
			m_CApply.EnableWindow(FALSE);
			}
		}	
	}

void CAddTurnpointDlg::OnKeydownListControl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	if( pLVKeyDow->wVKey==DELETE_KEY ) 
		{
		OnDelete();
		}
	*pResult = 0;
}

void CAddTurnpointDlg::OnKillfocusName() 
{
//		m_CApply.EnableWindow(TRUE);	
}

void CAddTurnpointDlg::OnChangeName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
		int iID=GetID();
		if( iID<0 ) return;

		UpdateData(TRUE);
		CTurnpoint* pcTurnpointItem = m_pturnpointArray->GetAt(iID-1);
		if( pcTurnpointItem )
			m_CApply.SetWindowText(_T("Apply"));
		else
			m_CApply.SetWindowText(_T("New"));

		m_CApply.EnableWindow(TRUE);
	
}

void CAddTurnpointDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	
}

void CAddTurnpointDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CAddTurnpointDlg::OnOK() 
{
	if(m_pturnpointArray->GetValidTpts()>0 && m_pturnpointArray->GetHomePoint()==NULL )
		{
		if( AfxMessageBox(_T("You should have at least on control point with the H (home) attribute set."), MB_OKCANCEL) !=IDOK ) return;		
		}
	
	if( m_bTasks )
		if( AfxMessageBox( _T("You have already defined one or more tasks.  Changing the control point list could cause BIG TROUBLE!!!\n\n Proceed at own risk?."),
							MB_OKCANCEL )!=IDOK ) return;

	CDialog::OnOK();
}

void CAddTurnpointDlg::OnChangeTurnpointId() 
{
	int iID=GetID();
	m_CID.GetWindowText(m_strID);
	CTurnpoint* pcTurnpointItem = m_pturnpointArray->GetAt(iID-1);
	UpdateFormFromTurnpoint(pcTurnpointItem);
	UpdateData(FALSE);	

	if( pcTurnpointItem )
		{
		for( int iItem=0; iItem<m_cTurnpointListControl.GetItemCount(); iItem++ )
			{
			if( pcTurnpointItem==(CTurnpoint*)m_cTurnpointListControl.GetItemData(iItem) )
				{
				m_cTurnpointListControl.Select(iItem);
				m_cTurnpointListControl.EnsureVisible(iItem, true);
				break;
				}
			}
		}
	else
		m_cTurnpointListControl.DeselectAll();

	m_CApply.EnableWindow(m_strID.GetLength()>0 && iID>0);

}

int CAddTurnpointDlg::GetID()
	{
	CString strCID;
	m_CID.GetWindowText(strCID);
	return _ttoi(strCID);
	}

