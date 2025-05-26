//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestantDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "wsclass.h"
#include "Contestant.h"
#include "ContestantDlg.h"
#include "ContestantAddress.h"
#include "GliderInfoList.h"
#include "HandicapCalcDlg.h"
#include "ContestantsFromSSA.h"
#include <fstream>
#include "global_prototypes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int fChangedNo=FALSE;
static int fModified=FALSE;
static int iSortColumn=0;
static  CStringArray strModelArray;

/////////////////////////////////////////////////////////////////////////////
// CContestantDlg dialog


CContestantDlg::CContestantDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CContestantDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContestantDlg)
	m_strContestNo = _T("");
	m_strFirstName = _T("");
	m_dHandicap = 1.0;
	m_strGlider = _T("");
	m_strLastName = _T("");
	m_strMiddleName = _T("");
	m_fFDRecorderUsed = FALSE;
	m_iSSANumber = 0;
	m_bMotorized = FALSE;
	m_bWithdrew = FALSE;
	m_bGuest = FALSE;
	m_bForeign = FALSE;
	//}}AFX_DATA_INIT

	 m_strAddress1="";
	 m_strAddress2="";
	 m_strCity="";
	 m_strState="";
	 m_strZipcode1="";
	 m_strZipcode2="";
	 m_bGuest=false;
	 m_bForeign=false;
	 m_pPreselect= NULL;

	m_fSpan=0;
	m_bWinglets		=false;
	m_bTurbulation	=false;
	m_bRootFairings	=false;
	m_fWeight=0;

    SetControlInfo(IDOK,                 ANCHORE_BOTTOM);
    SetControlInfo(IDC_APPLY,            ANCHORE_BOTTOM);
    SetControlInfo(IDC_IMPORT,           ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,             ANCHORE_BOTTOM);
    SetControlInfo(IDC_DELETE,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_CONTROL_BOX,    RESIZE_HOR | RESIZE_VER);
}


void CContestantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContestantDlg)
	DDX_Control(pDX, IDC_HCLOOKUPBUTTON, m_cHandicapLookupButton);
	DDX_Control(pDX, IDC_GLIDER_MAKE, m_cGliderMakeCombo);
	DDX_Control(pDX, IDC_CONTROL_BOX, m_cCListCtrlBox);
	DDX_Control(pDX, IDC_DELETE, m_CDelete);
	DDX_Control(pDX, IDC_LAST_NAME, m_CLastName);
	DDX_Control(pDX, IDC_CONTEST_NO, m_CContestNo);
	DDX_Control(pDX, IDC_GLIDER_HANDICAP, m_cHandicapEditBox);
	DDX_Control(pDX, IDC_HANDICAP_TEXT, m_cHandicapText);
	DDX_Control(pDX, IDC_CLASS_COMBO, m_cClassCombo);
	DDX_Text(pDX, IDC_CONTEST_NO, m_strContestNo);
	DDV_MaxChars(pDX, m_strContestNo, 3);
	DDX_Text(pDX, IDC_FIRST_NAME, m_strFirstName);
	DDV_MaxChars(pDX, m_strFirstName, 20);
	DDX_Text(pDX, IDC_GLIDER_HANDICAP, m_dHandicap);
	DDX_Text(pDX, IDC_GLIDER_MAKE, m_strGlider);
	DDX_Text(pDX, IDC_FDR_ID, m_strFDR_ID);
	DDX_Text(pDX, IDC_LAST_NAME, m_strLastName);
	DDV_MaxChars(pDX, m_strLastName, 20);
	DDX_Text(pDX, IDC_MIDDLE_NAME, m_strMiddleName);
	DDV_MaxChars(pDX, m_strMiddleName, 10);
	DDX_Text(pDX, IDC_SSA_NUMBER, m_iSSANumber);
	DDV_MinMaxInt(pDX, m_iSSANumber, 0, 10000000);
	DDX_Check(pDX, IDC_MOTORCHECK, m_bMotorized);
	DDX_Check(pDX, IDC_WITHDREW_CHECK, m_bWithdrew);
	DDX_Check(pDX, IDC_GUEST_CHECK, m_bGuest);
	DDX_Check(pDX, IDC_CHECK1, m_bForeign);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContestantDlg, CDialog)
	//{{AFX_MSG_MAP(CContestantDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_CBN_SELCHANGE(IDC_CLASS_COMBO, OnSelchangeClassCombo)
	ON_BN_CLICKED(IDC_CONTESTANT_PERSONAL, OnContestantPersonal)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_FDR_BUTTON, OnFdrButton)
	ON_BN_CLICKED(IDC_RECORDER_CHECK, OnRecorderCheck)
	ON_EN_KILLFOCUS(IDC_CONTEST_NO, OnKillfocusContestNo)
	ON_EN_CHANGE(IDC_CONTEST_NO, OnChangeContestNo)
	ON_EN_SETFOCUS(IDC_CONTEST_NO, OnSetfocusContestNo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONTROL_BOX, OnItemchangedControlBox)
	ON_NOTIFY(NM_KILLFOCUS, IDC_CONTROL_BOX, OnKillfocusControlBox)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CONTROL_BOX, OnColumnclickControlBox)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CONTROL_BOX, OnKeydownControlBox)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_CONTROL_BOX, OnItemclickControlBox)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_HCLOOKUPBUTTON, OnHclookupbutton)
	ON_BN_CLICKED(IDC_CHECK1, OnForeignCheck)
	ON_BN_CLICKED(IDC_IMPORT, OnImportSSA)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContestantDlg message handlers

BOOL CContestantDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	//Load the glider selector with all the models in the handicap list
	int nModels=strModelArray.GetCount();
	if( nModels==0 ) 
		{
		int iRead=m_pcWinscoreDoc->m_cGIList.GetCount();
		if( iRead==0 ) 
			{
			//Cached copy empty, read from local file
			iRead=m_pcWinscoreDoc->m_cGIList.ImportXML(GetHandicapFileName());
			}
		if( iRead>0 ) 
			{
			nModels=m_pcWinscoreDoc->m_cGIList.GetAllModels(strModelArray);
			}
		}
	for( int iModel=0; iModel<nModels; iModel++)
		{
		m_cGliderMakeCombo.AddString(strModelArray[iModel]);
		}

	if( m_pcWinscoreDoc->GetNumClasses()==1 ) 
		m_cClassCombo.EnableWindow(false);

	m_cCListCtrlBox.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	m_pcontestantList->CreateControlColumns(m_cCListCtrlBox);

	m_pcWinscoreDoc->SetAvailableClasses(&m_cClassCombo);
	UpdateHandicapFields();
	UpdateListBox();
	fChangedNo=FALSE;
	fModified=FALSE;
	iSortColumn=0;
	m_CDelete.EnableWindow(FALSE);

	if( m_pPreselect>=0 ) 
		{
		UpdateFromContestant(m_pPreselect);
		}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CContestantDlg::OnApply() 
	{

	if( UpdateData()==0 ) return;   

    m_strContestNo.TrimRight();
    m_strContestNo.TrimLeft();
    m_strContestNo.MakeUpper();

	if( m_strContestNo=="" )
		{
		AfxMessageBox( IDS_ERR_INVALID_CONTEST_NO, MB_OK  );
		m_CContestNo.SetFocus();
		return;
		}

	if( m_strContestNo.GetLength()==3 && m_strContestNo[1]==' ' )
		{
		AfxMessageBox(_T("Invalid Contest ID, no blanks allowed. "), MB_OK  );
		m_CContestNo.SetFocus();
		return;
		}


	if( m_strLastName==_T("") )
		{
		AfxMessageBox( IDS_ERR_INVALID_PILOT_NAME, MB_OK  );
		m_CLastName.SetFocus();
		return;
		}
	if( m_strGlider==_T("") )
		{
		AfxMessageBox( IDS_ERR_INVALID_GLIDER, MB_OK  );
		m_cGliderMakeCombo.SetFocus();
		return;
		}

	CContestant* pcNewContestant = new CContestant(	
					 m_strContestNo,
					 m_strLastName,
					 m_strMiddleName,
					 m_strFirstName,
					 m_strGlider,
					 m_strAddress1,
					 m_strAddress2,
					 m_strCity,
					 m_strState,
					 m_strZipcode1,
					 m_strZipcode2,
					 m_strFDR_ID,
					 m_iSSANumber,
					 m_dHandicap,
					 m_eClass,
					 (m_bGuest || m_bForeign),
					 m_bMotorized,
					 m_bWithdrew,
					 false,
					 m_bForeign);

	if( !m_cGliderInfo.IsNull() )
		{
		pcNewContestant->m_cGliderInfo	=m_cGliderInfo;
		pcNewContestant->m_fSpan		=m_fSpan;

		if( m_bWinglets )
			pcNewContestant->SetOption(CON_WINGLETS);
		else
			pcNewContestant->ClearOption(CON_WINGLETS);
		if( m_bTurbulation )
			pcNewContestant->SetOption(CON_TURBULATION);
		else
			pcNewContestant->ClearOption(CON_TURBULATION);
		if( m_bRootFairings )
			pcNewContestant->SetOption(CON_ROOTFARINGS);
		else
			pcNewContestant->ClearOption(CON_ROOTFARINGS);

		pcNewContestant->m_fWeight		=m_fWeight;
		}

	m_pcontestantList->AddToList(pcNewContestant);

	bool bUpdated=FALSE;
	for( int i=0; i<m_cCListCtrlBox.GetItemCount(); i++ )
		{
		if( m_cCListCtrlBox.GetItemText(i,1)==m_strContestNo )
			{
			m_pcontestantList->SetContestantItem(	m_cCListCtrlBox, 
													i, 
													pcNewContestant);
			bUpdated=TRUE;
			}
		}

	if( !bUpdated ) 
		{
		// Add the entry to the top of the list
		//UpdateListBox();
		m_cCListCtrlBox.InsertItem( 0, _T("") );
		m_pcontestantList->SetContestantItem(	m_cCListCtrlBox, 
												0, 
												pcNewContestant);
		m_cCListCtrlBox.EnsureVisible(0, true);

		}

	for( int iItem=0; iItem<m_cCListCtrlBox.GetItemCount(); iItem++)
		{
		if( pcNewContestant==(CContestant*)m_cCListCtrlBox.GetItemData(iItem) )
			{
			m_cCListCtrlBox.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
			m_cCListCtrlBox.SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
			m_cCListCtrlBox.Update(iItem);
			m_cCListCtrlBox.EnsureVisible(iItem, true);
			m_CDelete.EnableWindow(TRUE);	
			break;
			}
		}

	m_CContestNo.SetSel( 0,3 );

	m_CContestNo.SetFocus();
	UpdateData(FALSE);
	fChangedNo=FALSE;
	fModified=TRUE;	
	}



void CContestantDlg::OnSelchangeClassCombo() 
{
	UpdateHandicapFields();
}



void CContestantDlg::OnKillfocusContestNo() 
{
	if( !fChangedNo ) return;

	UpdateData();

    m_strContestNo.TrimRight();
    m_strContestNo.TrimLeft();
    m_strContestNo.MakeUpper();

	CContestant* pcContestant= 
		m_pcontestantList->GetByContestNo(m_strContestNo);
	
	if( pcContestant ) 
		{
		UpdateFromContestant(pcContestant );
		}
	else
		{
		CString ctmp=m_strContestNo;
		ResetContestData();
		m_strContestNo=ctmp;
		UpdateData(FALSE);
		}
	fChangedNo=FALSE;
	m_CDelete.EnableWindow(FALSE);	
}
/*
void CContestantDlg::OnSelchangeContestantList() 
{
	// TODO: Add your control notification handler code here
	UpdateFromListboxSel();
	m_CDelete.EnableWindow(TRUE);
}
*/
void CContestantDlg::OnContestantPersonal() 
{
	CContestantAddress	dlg;

	dlg.m_strAddress1=m_strAddress1;
	dlg.m_strAddress2=m_strAddress2;
	dlg.m_strCity=m_strCity;
	dlg.m_strState=m_strState;
	dlg.m_strZip1=m_strZipcode1;
	dlg.m_strZip2=m_strZipcode2;

	if (dlg.DoModal() == IDOK)
		{
		UpdateData(); 
		m_strAddress1=dlg.m_strAddress1;
		m_strAddress2=dlg.m_strAddress2;
		m_strCity=dlg.m_strCity;
		m_strState=dlg.m_strState;
		m_strZipcode1=dlg.m_strZip1;
		m_strZipcode2=dlg.m_strZip2;

		// Now update the contestants address in the list.
		CContestant* pContestant=m_pcontestantList->GetByContestNo(m_strContestNo);
		if( pContestant )
			{
			pContestant->m_strAddress1=dlg.m_strAddress1;
			pContestant->m_strAddress2=dlg.m_strAddress2;
			pContestant->m_strCity=dlg.m_strCity;
			pContestant->m_strState=dlg.m_strState;
			pContestant->m_strZipcode1=dlg.m_strZip1;
			pContestant->m_strZipcode2=dlg.m_strZip2;
			fModified=TRUE;
			UpdateListBox();
			}
		}
}


void CContestantDlg::OnFdrButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CContestantDlg::OnRecorderCheck() 
{
	// TODO: Add your control notification handler code here
	
}

void CContestantDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if( fModified )
		{	
		int iAnswer=AfxMessageBox( _T("Save changes to contestants?"), MB_YESNOCANCEL  );
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


void CContestantDlg::UpdateListBox()
	{
	m_pcontestantList->LoadContestantList( m_cCListCtrlBox );
//	m_cCListCtrlBox.SortItems( CompareContestantName, iSortColumn+1 );
	}


void CContestantDlg::SetContestantList(CContestantList * pcContestantList)
{
	m_pcontestantList=pcContestantList;
}



void CContestantDlg::SetDocument(CWinscoreDoc* pcInputWinscoreDoc)
{
	m_pcWinscoreDoc=pcInputWinscoreDoc;
}


void CContestantDlg::UpdateHandicapFields()
{
	int iSel=m_cClassCombo.GetCurSel();
	m_eClass = (EClass)m_cClassCombo.GetItemData(iSel);
	if( !GetClass(m_eClass).IsHandicapped() )
		{
		m_cHandicapText.EnableWindow(FALSE);
		m_cHandicapEditBox.EnableWindow(FALSE);
		m_cHandicapLookupButton.EnableWindow(FALSE);
		}
	else
		{
		m_cHandicapText.EnableWindow(TRUE);
		m_cHandicapEditBox.EnableWindow(TRUE);
		m_cHandicapLookupButton.EnableWindow(TRUE);
		}
}

void CContestantDlg::ResetContestData()
{
	m_strContestNo = _T("");
	m_strFirstName = _T("");
	m_dHandicap = 1.0;
	m_strGlider = _T("");
	m_strLastName = _T("");
	m_strMiddleName = _T("");
	m_fFDRecorderUsed = FALSE;
	m_iSSANumber = 0;
	m_strAddress1=_T("");
	m_strAddress2=_T("");
	m_strCity=_T("");
	m_strState=_T("");
	m_strZipcode1=_T("");
	m_strZipcode2=_T("");
	m_bGuest=false;	
	m_bMotorized = FALSE;
	m_cGliderInfo.m_bNull=true;
	m_CDelete.EnableWindow(FALSE);

}

void CContestantDlg::UpdateFromListboxSel()
	{
	int iSel = m_cCListCtrlBox.GetNextItem( -1, LVNI_SELECTED );
	if( iSel <0 ) return;

	CContestant* pcContestant=(CContestant*)m_cCListCtrlBox.GetItemData(iSel);
	if( pcContestant ) UpdateFromContestant(pcContestant);
	return;

	}

void CContestantDlg::UpdateFromContestant( CContestant* pcContestant )
	{
	if( !pcContestant ) return;

	m_strContestNo = pcContestant->m_strContestNo;
	m_strFirstName = pcContestant->m_strFirstName;
	m_dHandicap = pcContestant->m_fHandicap;
	m_strGlider = pcContestant->m_strGlider;
	m_strLastName = pcContestant->m_strLastName;
	m_strMiddleName = pcContestant->m_strMiddleName;
	m_fFDRecorderUsed = FALSE;
	m_iSSANumber =pcContestant->m_iSSANumber;
	m_strAddress1=pcContestant->m_strAddress1;
	m_strAddress2=pcContestant->m_strAddress2;
	m_strCity=pcContestant->m_strCity;
	m_strState=pcContestant->m_strState;
	m_strZipcode1=pcContestant->m_strZipcode1;
	m_strZipcode2=pcContestant->m_strZipcode2;
	m_strFDR_ID=pcContestant->m_strFDR_ID;
	m_bGuest=pcContestant->IsGuest();
	m_bForeign=pcContestant->IsForeign();
	m_eClass=pcContestant->m_eClass;

	m_bWinglets		=pcContestant->CheckOption(CON_WINGLETS)?true:false;
	m_bTurbulation	=pcContestant->CheckOption(CON_TURBULATION)?true:false;
	m_bRootFairings	=pcContestant->CheckOption(CON_ROOTFARINGS)?true:false;

	m_fSpan		 =pcContestant->m_fSpan;
	m_fWeight	 =pcContestant->m_fWeight;
	m_cGliderInfo=pcContestant->m_cGliderInfo;

	m_bMotorized=pcContestant->IsMotorized();
	m_bWithdrew =pcContestant->HasWithdrawn();

	UpdateClassCombo();
	UpdateHandicapFields();
	UpdateData(FALSE);
	}


void CContestantDlg::UpdateClassCombo()
	{
	int iCount=m_cClassCombo.GetCount();
	if( iCount<=1 ) return;

	for( int i=0; i<iCount; i++)
		{
		if( m_eClass == (EClass)m_cClassCombo.GetItemData(i) )
			{
			m_cClassCombo.SetCurSel(i);
			return;
			}
		}
	}



void CContestantDlg::OnChangeContestNo() 
{
	int iStart, iEnd;
	m_CContestNo.GetWindowText(m_strContestNo);
	m_CContestNo.GetSel(iStart, iEnd);

    m_strContestNo.TrimRight();
    m_strContestNo.TrimLeft();
    m_strContestNo.MakeUpper();

	CContestant* pcContestant= 
		m_pcontestantList->GetByContestNo(m_strContestNo);
	
	if( pcContestant ) 
		{
		for( int iItem=0; iItem<m_cCListCtrlBox.GetItemCount(); iItem++)
			{
			if( pcContestant==(CContestant*)m_cCListCtrlBox.GetItemData(iItem) )
				{
				m_cCListCtrlBox.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
				m_cCListCtrlBox.SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
				m_cCListCtrlBox.Update(iItem);
				m_cCListCtrlBox.EnsureVisible(iItem, true);
				m_CDelete.EnableWindow(TRUE);	
				break;
				}
			}
		UpdateFromContestant(pcContestant );
		}
	else
		{
		CString ctmp=m_strContestNo;
		ResetContestData();
		m_strContestNo=ctmp;
		UpdateData(FALSE);
		m_CDelete.EnableWindow(FALSE);
		}
	fChangedNo=FALSE;

	m_CContestNo.SetSel(iStart, iEnd);

}

void CContestantDlg::OnSetfocusContestNo() 
{
	fChangedNo=FALSE;	
}


void CContestantDlg::OnItemchangedControlBox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ( pNMListView->uNewState & LVIS_SELECTED )
		{ 
		m_CDelete.EnableWindow(TRUE);
		UpdateFromListboxSel();
		}

	*pResult = 0;
}

void CContestantDlg::OnKillfocusControlBox(NMHDR* pNMHDR, LRESULT* pResult) 
{

//	m_CDelete.EnableWindow(FALSE);	
	*pResult = 0;
}



void CContestantDlg::OnDelete() 
{
	int iSel = m_cCListCtrlBox.GetNextItem( -1, LVNI_SELECTED );
	if( iSel<0) return;

    CContestant* pcContestant=(CContestant*)m_cCListCtrlBox.GetItemData(iSel);
	m_pcontestantList->RemoveByPointer(pcContestant);
	UpdateData(FALSE);
	UpdateListBox();
	
	m_cCListCtrlBox.SetItemState((iSel-1>=0)?(iSel-1):(0), LVIS_SELECTED, LVIS_SELECTED);
	m_cCListCtrlBox.SetItemState((iSel-1>=0)?(iSel-1):(0), LVIS_FOCUSED, LVIS_FOCUSED);
	m_cCListCtrlBox.Update((iSel-1>=0)?(iSel-1):(0));
		
	UpdateFromListboxSel();
	fModified=TRUE;	
	if( m_pcontestantList->IsEmpty() )m_CDelete.EnableWindow(FALSE);
}

void CContestantDlg::OnColumnclickControlBox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iSort=pNMListView->iSubItem+1;
	if( iSort==abs(iSortColumn)  )
		iSortColumn= -iSortColumn;
	else
		iSortColumn=iSort;

	m_cCListCtrlBox.SortItems( CompareContestantName, iSortColumn );
	*pResult = 0;
}

void CContestantDlg::OnKeydownControlBox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	if( pLVKeyDow->wVKey==DELETE_KEY ) 
		{
		OnDelete();
		}
	*pResult = 0;
}
/*
void CContestantDlg::OnImportContestant() 
{
	CString  cFileName="";

	static char BASED_CODE szFilter[] = "Winscore Contestant Files (*.wcf)|*.wcf|All Files (*.*)|*.*||";
	CFileDialog  cFileDlg(TRUE, "wcf", cFileName, OFN_HIDEREADONLY, szFilter);
	cFileDlg.m_ofn.lpstrTitle="Import Contestants";

	int iOriginalCount=m_pcontestantList->GetCount();

	if( cFileDlg.DoModal()==IDOK )
		{

		try
			{
			ifstream ifs( cFileDlg.GetPathName() );
			ifs >> *m_pcontestantList;
			ifs.close();
			}
		catch (...)
			{
			AfxMessageBox("Error Reading Contestant File: "+cFileDlg.GetPathName(), MB_ICONSTOP );
			return;
			}
		UpdateListBox();
		UpdateData(FALSE);
		fModified=1;
		CString cStatus;
	
		if( iOriginalCount<m_pcontestantList->GetCount() )
			{
			cStatus.Format("%d contestants successfully Imported from: %s", m_pcontestantList->GetCount()-iOriginalCount, cFileDlg.GetPathName());
			AfxMessageBox(  cStatus,	MB_OK | MB_ICONINFORMATION, 0 );
			}
		}		
}
*/
void CContestantDlg::OnItemclickControlBox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	*pResult = 0;
}

void CContestantDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CContestantDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CContestantDlg::OnHclookupbutton() 
{
	CGliderInfo *pDefault=NULL;
	UpdateData(true);

	int iRead=m_pcWinscoreDoc->m_cGIList.GetCount();
	if( iRead==0 ) 
		{
		//Cached copy empty, read from local file
		iRead=m_pcWinscoreDoc->m_cGIList.ImportXML(GetHandicapFileName());
		}

	if( iRead<=0 )
		{
		CString strError="Error reading handicap list from: ";
		strError+=GetHandicapFileName();
		AfxMessageBox(strError,MB_ICONSTOP);
		return;
		}

	if( m_cGliderInfo.IsNull() )
		pDefault=m_pcWinscoreDoc->m_cGIList.GetModel(m_strGlider);
	else
		pDefault= &m_cGliderInfo;

	CHandicapCalcDlg dlg(m_pcWinscoreDoc->m_cGIList, pDefault);

	dlg.m_strTitle=m_pcWinscoreDoc->m_cGIList.m_strTitle;

	if( !m_cGliderInfo.IsNull() )
		{
		dlg.m_fSpan			= m_fSpan;

		dlg.m_bWingletCheck		= m_bWinglets;
		dlg.m_bTurbulationCheck	= m_bTurbulation;
		dlg.m_bRootFaringCheck = m_bRootFairings;

		dlg.m_fWeight		= m_fWeight;
		}
	else
		{
		dlg.m_fSpan				= 0;
		dlg.m_bWingletCheck		= false;
		dlg.m_bTurbulationCheck	= false;
		dlg.m_bRootFaringCheck = false;
		dlg.m_fWeight			= 0;
		}
	dlg.m_dHandicap		= m_dHandicap;
	
	if( dlg.DoModal()==IDOK )
		{
		if( dlg.m_pcGliderInfo )
			m_cGliderInfo= *dlg.m_pcGliderInfo;

		m_fSpan		 =  dlg.m_fSpan;

		m_bWinglets		= dlg.m_bWingletCheck?1:0;
		m_bTurbulation	= dlg.m_bTurbulationCheck?1:0;
		m_bRootFairings	= dlg.m_bRootFaringCheck?1:0;

		m_fWeight	 =  dlg.m_fWeight;
		m_dHandicap	 =	dlg.m_dHandicap;

		m_strGlider=dlg.m_pcGliderInfo->m_strModel;
		
		if( dlg.m_pcGliderInfo->m_strNotes.Find("M")>=0 )
			{
			m_bMotorized=true;
			}
		else
			m_bMotorized=false;

		UpdateData(false);

		}
}	

void CContestantDlg::OnForeignCheck() 
{
	
}


void CContestantDlg::OnImportSSA() 
{
	CContestantsFromSSA cDlg;
	cDlg.m_pDoc=m_pcWinscoreDoc;
	if (cDlg.DoModal() == IDOK)
		{

		POSITION pos = cDlg.m_cContestantList.GetHeadPosition();

		while(1)
			{
			CContestant *pcContestant=(CContestant*)cDlg.m_cContestantList.GetNext(pos);
			if( pcContestant ) 
				{
				if( !m_pcontestantList->GetByContestNo(pcContestant->m_strContestNo) ) 
					{
					m_pcontestantList->AddTail(pcContestant);
					}
				}
			if( pos==NULL ) break;
			}
		cDlg.m_cContestantList.RemoveAll();	
		m_pcontestantList->LoadContestantList( m_cCListCtrlBox );

		}
}
