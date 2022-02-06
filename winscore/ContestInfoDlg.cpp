//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "wsversion.h"
#include "ContestInfoDlg.h"
#include "DescriptionDlg.h"
#include "ClassPropSheet.h"
#include "contestsettingsdlg.h"
#include "SSAPicker.h"
#include "Global_Prototypes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContestInfoDlg dialog


CContestInfoDlg::CContestInfoDlg(CWinscoreDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CContestInfoDlg::IDD, pParent),
	m_pDoc(pDoc)
	, m_iNumPracticeDays(2)
{
	//{{AFX_DATA_INIT(CContestInfoDlg)
	m_strLocation = _T("");
	m_strContestName = _T("");
	m_iNumDays = 1;
	m_cContestStartDate = 0;
	//}}AFX_DATA_INIT

	m_eContest=eRegional;
	m_eUnits=eStatute;
    m_eDatum=DEFAULTDATUM;
}


void CContestInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContestInfoDlg)
	DDX_Control(pDX, IDC_CLASSPROPERTIESBUTTON, m_cClassPropButton);
	DDX_Control(pDX, IDC_CLASSLIST, m_cClassListCtrl);
	DDX_Control(pDX, IDC_REGIONAL_RADIO, m_CRegionalButton);
	DDX_Control(pDX, IDC_NATIONAL_RADIO, m_CNationalButton);
	DDX_Text(pDX, IDC_CONTEST_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_CONTEST_NAME, m_strContestName);
	DDX_Text(pDX, IDC_NUMBER_DAYS, m_iNumDays);
	DDV_MinMaxInt(pDX, m_iNumDays, 1, 300);
	DDX_Text(pDX, IDC_NUMBER_PRACTICE_DAYS, m_iNumPracticeDays);
	DDV_MinMaxInt(pDX, m_iNumPracticeDays, 1, 14);
	DDX_DateTimeCtrl(pDX, IDC_STARTDATEPICKER, m_cContestStartDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContestInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CContestInfoDlg)
	ON_BN_CLICKED(IDC_NATIONAL_RADIO, OnNationalRadio)
	ON_BN_CLICKED(IDC_REGIONAL_RADIO, OnRegionalRadio)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_STARTDATEPICKER, OnDatetimechangeStartdatepicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_CLASSLIST, OnKillfocusClasslist)
	ON_NOTIFY(NM_CLICK, IDC_CLASSLIST, OnClickClasslist)
	ON_BN_CLICKED(IDC_CLASSPROPERTIESBUTTON, OnClasspropertiesbutton)
	ON_NOTIFY(NM_DBLCLK, IDC_CLASSLIST, OnDblclkClasslist)
	ON_BN_CLICKED(IDC_CONTEST_SETTINGS, OnContestSettings)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SSA_BUTTON, &CContestInfoDlg::OnBnClickedSsaButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContestInfoDlg message handlers


void CContestInfoDlg::SetContest(EContest eContest)
{
	m_eContest=eContest;
	m_CRegionalButton.SetCheck(eContest==eRegional);
	m_CNationalButton.SetCheck(eContest==eNational);
}

void CContestInfoDlg::OnNationalRadio() 
{
	SetContest(eNational);
}

void CContestInfoDlg::OnRegionalRadio() 
{
	SetContest(eRegional);	
}






BOOL CContestInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetContest(m_eContest);
	
	m_cClassListCtrl.AddColumn("Classes in Contest");
	m_cClassListCtrl.SetAvailableClasses( NUMCLASSES, m_acClass, m_eContest, true );

	m_cClassPropButton.EnableWindow(false);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CContestInfoDlg::OnOK() 
{

	UpdateData();

	if ( m_cContestStartDate.GetYear() > CURRENTYEAR )
		{
		CString strError;
		strError.Format(_T("This version of Winscore can only be used for the %d contest season. "),CURRENTYEAR);
		AfxMessageBox( strError );
		return;
		}

	if( m_strContestName==_T("") )
		{
		AfxMessageBox( IDS_ERR_NO_CONTEST_NAME,  MB_OK );
		return;
		}

	if( m_cClassListCtrl.GetNumCheckedClasses()==0 )
		{
		  AfxMessageBox( IDS_ERR_NO_CLASSES,  MB_OK );
		  return;
		};


	for(int i=0; i<NUMCLASSES; i++ )
		{
		if( m_cClassListCtrl.IsChecked( m_acClass[i].GetType()) && m_acClass[i].m_eContest==m_eContest )
			m_acClass[i].SetActive( true );
		else
			m_acClass[i].SetActive( false );			
		}

	CDialog::OnOK();
}


void CContestInfoDlg::OnDatetimechangeStartdatepicker(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CContestInfoDlg::OnKillfocusClasslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	m_cClassPropButton.EnableWindow(false);
	*pResult = 0;
}

void CContestInfoDlg::OnClickClasslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_cClassPropButton.EnableWindow(true);
	*pResult = 0;
}

void CContestInfoDlg::OnClasspropertiesbutton() 
{
	EditClass();
}

void CContestInfoDlg::OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EditClass();
	*pResult = 0;
}

void CContestInfoDlg::EditClass()
{
	EClass eClass;
	if( m_cClassListCtrl.GetSelectedClass( eClass ) )
		{
		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			{
			if( m_acClass[iClass].GetType()==eClass &&
				m_acClass[iClass].m_eContest==m_eContest )
				{

				CClassPropSheet cClassSheet(m_acClass[iClass].GetName()+_T(" Properties"), m_acClass[iClass], m_eUnits );
				if( cClassSheet.DoModal()== IDOK )
					{
					m_cClassListCtrl.SetAvailableClasses(NUMCLASSES, m_acClass, m_eContest, true );
					}
				break;
				}
			}

		}	
}

void CContestInfoDlg::OnContestSettings() 
{
	CContestSettingsDlg	dlg;
	dlg.m_iDistanceUnits	=m_eUnits;
	dlg.m_iLatLongFormat	=m_eCoordinateFormat;
	dlg.m_iDatum			=m_eDatum;
	dlg.m_bGuestsAtBottom	=GetWinscoreInt( GUESTPOSITION, 0 );
	dlg.m_bTwoGateCheck		=GetWinscoreInt( ALLOWTWOGATES, 0 );

	if( dlg.DoModal()==IDOK )
		{
		m_eUnits			=(EUnits)dlg.m_iDistanceUnits;
		m_eCoordinateFormat	=(ECoordFormat)dlg.m_iLatLongFormat;
		m_eDatum			=(EDatum)dlg.m_iDatum;
		WriteWinscoreInt( GUESTPOSITION, (int)dlg.m_bGuestsAtBottom );
		WriteWinscoreInt( ALLOWTWOGATES, (int)dlg.m_bTwoGateCheck );
		}

}



void CContestInfoDlg::OnBnClickedSsaButton()
{
	CSSAPicker dlg;

	if( dlg.DoModal()==IDOK )
		{
		m_iSSA=dlg.m_iSSA;

		CString strURL;
		strURL.Format(_T("https://members.ssa.org/cde/winscore_xml.asp?ContestId=%d"),dlg.m_iSSA);
		try
			{
			CWaitCursor cWait;
			CXMLMgr cMgr;
			cMgr.Initlize();

			if( !cMgr.Load(strURL) )
				{
				AfxMessageBox( _T("Contest information at ssa.org could not be downloaded.\n\nMake sure one or more contestants are marked as \"Confirmed\""),  MB_ICONSTOP   );
				return;
				}

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") )
				{
				AfxMessageBox( _T("Contest information at ssa.org could not be downloaded.\n\nMake sure one or more contestants are marked as \"Confirmed\""),  MB_ICONSTOP   );
				return;
				}

			GET_XML_STR( cMgr, cMgr.GetRoot(), _T("Name"),		(m_strContestName) );
			GET_XML_STR( cMgr, cMgr.GetRoot(), _T("Location"),	(m_strLocation) );
			GET_XML_INT( cMgr, cMgr.GetRoot(), _T("Contesttype"),	EContest,	m_eContest, eRegional );
			m_CRegionalButton.SetCheck(m_eContest==eRegional);
			m_CNationalButton.SetCheck(m_eContest==eNational);

			GET_XML_INT( cMgr, cMgr.GetRoot(), _T("NumberOfDays"),	int,	m_iNumDays, 6 );
			MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode= cMgr.GetRoot();
			ImportXMLDate(cMgr, pIDOMWinscoreNode, _T("startdate"), m_cContestStartDate);

			MSXML2::IXMLDOMNodePtr pClasses;
			TESTHR(cMgr.SelectChild(_T("Classes"),pIDOMWinscoreNode,pClasses));
			if( pClasses==NULL ) return;

			MSXML2::IXMLDOMNodeListPtr pClassList=cMgr.GetChildList(pClasses );
			if( pClassList==NULL ) return;

			long length=0;
			TESTHR(pClassList->get_length(&length));
			if( length==0 ) return;

			for( int i=0; i<length; i++ )
				{
				MSXML2::IXMLDOMNodePtr pClass;
				TESTHR(pClassList->get_item(i,&pClass));\
				CWSClass cClass;
				cClass.ImportXML(cMgr, pClass);
				
				CString str=cClass.GetName();
				m_cClassListCtrl.CheckClass(str);

				}

			UpdateData(FALSE);

			if( CheckForHandicapUpdate() )
				{
				if( AfxMessageBox( "Contest information successfully imported.\n\nAn updated handicap list is available from ssa.org.\n\nUpdate handicap list now?", MB_ICONQUESTION  | MB_YESNO )==IDYES ) 
				{
				UpdateHandicapListFromSSA(m_pDoc->m_cGIList);
					}
				}
			else
				{
				AfxMessageBox( "Contest information successfully imported.", MB_ICONINFORMATION );
				}

			}
		catch (...)
			{
			AfxMessageBox( _T("Unexpected error reading contestant information from members.ssa.org."),  MB_ICONSTOP   );
			}
	}

}
