// Contestant1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "Contestant1Dlg.h"
#include "afxdialogex.h"
#include "GliderInfoList.h"
#include "HandicapCalcDlg.h"
#include "Global_Prototypes.h"
#include "ContestantAddress.h"

static  CStringArray strModelArray;

// CContestant1Dlg dialog

IMPLEMENT_DYNAMIC(CContestant1Dlg, CDialogEx)

CContestant1Dlg::CContestant1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContestant1Dlg::IDD, pParent)
{

}

CContestant1Dlg::~CContestant1Dlg()
{
}

void CContestant1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CContestantDlg)
	DDX_Control(pDX, IDC_HCLOOKUPBUTTON, m_cHandicapLookupButton);
	DDX_Control(pDX, IDC_GLIDER_MAKE, m_cGliderMakeCombo);
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


BEGIN_MESSAGE_MAP(CContestant1Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_APPLY, &CContestant1Dlg::OnOK)
	ON_CBN_SELCHANGE(IDC_CLASS_COMBO, &CContestant1Dlg::OnSelchangeClassCombo)
	ON_BN_CLICKED(IDC_HCLOOKUPBUTTON, &CContestant1Dlg::OnClickedHclookupbutton)
	ON_BN_CLICKED(IDC_CONTESTANT_PERSONAL, &CContestant1Dlg::OnBnClickedContestantPersonal)
	ON_CBN_SELCHANGE(IDC_GLIDER_MAKE, &CContestant1Dlg::OnCbnSelchangeGliderMake)
END_MESSAGE_MAP()


// CContestant1Dlg message handlers


void CContestant1Dlg::OnOK()
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

	if( m_pContestant!=NULL && m_pContestant->m_strContestNo!=m_strContestNo)
		{
		CContestant *pCheck=m_pcWinscoreDoc->m_contestantList.GetByContestNo(m_strContestNo);
		if( pCheck )
			{
			AfxMessageBox("That contest ID is already used by another contestant.");
			return;
			}
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

	m_pNewContestant = new CContestant(	
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
		m_pNewContestant->m_cGliderInfo	=m_cGliderInfo;
		m_pNewContestant->m_fSpan		=m_fSpan;

		if( m_bWinglets )
			m_pNewContestant->SetOption(CON_WINGLETS);
		else
			m_pNewContestant->ClearOption(CON_WINGLETS);
		if( m_bTurbulation )
			m_pNewContestant->SetOption(CON_TURBULATION);
		else
			m_pNewContestant->ClearOption(CON_TURBULATION);
		if( m_bRootFairings )
			m_pNewContestant->SetOption(CON_ROOTFARINGS);
		else
			m_pNewContestant->ClearOption(CON_ROOTFARINGS);

		m_pNewContestant->m_fWeight		=m_fWeight;
		}

	if( GetClass(m_eClass).IsHandicapped() )
		{
		if( m_dHandicap<.1 || m_dHandicap>2.3 )
			{
			CString strMess;
			strMess.Format("Handicap of %f is not reasonable.  Continue anyway?",m_dHandicap);
			if( AfxMessageBox( _T(strMess), MB_YESNOCANCEL  )!=IDYES ) return;
			}
		}


	CDialogEx::OnOK();
}


BOOL CContestant1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if( m_pcWinscoreDoc->GetNumClasses()==1 ) 
		m_cClassCombo.EnableWindow(false);

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
	
	m_pcWinscoreDoc->SetAvailableClasses(&m_cClassCombo);
	UpdateHandicapFields();


	if( m_pContestant ) 
		{
		UpdateFromContestant(m_pContestant);
		CString str;
		str.Format("Edit Contestant %s",m_strContestNo);
		SetWindowText(str);
		//m_CContestNo.SetReadOnly(true);
		}
	else
		{
		SetWindowText("Add New Contestant");
		m_dHandicap=0.0;
		m_iSSANumber=0;
		UpdateData(FALSE);
		}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CContestant1Dlg::UpdateFromContestant( CContestant* pcContestant )
	{
	if( !pcContestant ) return;

	m_strContestNo = pcContestant->m_strContestNo;
	m_strFirstName = pcContestant->m_strFirstName;
	m_dHandicap = pcContestant->m_fHandicap;
	m_strGlider = pcContestant->m_strGlider;
	m_strLastName = pcContestant->m_strLastName;
	m_strMiddleName = pcContestant->m_strMiddleName;
	m_iSSANumber =pcContestant->m_iSSANumber;
	m_strAddress1=pcContestant->m_strAddress1;
	m_strAddress2=pcContestant->m_strAddress2;
	m_strCity=pcContestant->m_strCity;
	m_strState=pcContestant->m_strState;
	m_strZipcode1=pcContestant->m_strZipcode1;
	m_strZipcode2=pcContestant->m_strZipcode2;
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

void CContestant1Dlg::UpdateHandicapFields()
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

void CContestant1Dlg::UpdateClassCombo()
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

void CContestant1Dlg::OnSelchangeClassCombo()
{
		UpdateHandicapFields();
}


void CContestant1Dlg::OnClickedHclookupbutton()
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


void CContestant1Dlg::OnBnClickedContestantPersonal()
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
		}
}


void CContestant1Dlg::OnCbnSelchangeGliderMake()
{
	int iSel= -1;
	CGliderInfo *pDefault=NULL;
	UpdateData();

	iSel=m_cGliderMakeCombo.GetCurSel();
	CString strMake;
	m_cGliderMakeCombo.GetLBText(iSel,strMake);
	pDefault=m_pcWinscoreDoc->m_cGIList.GetModel(strMake);
	if( pDefault )
		{
		m_cGliderInfo= *pDefault;
		m_fSpan			=  m_cGliderInfo.m_fSpan;
		m_fWeight		=  m_cGliderInfo.m_fWeight;
		m_dHandicap		=  m_cGliderInfo.m_dHandicap;
		m_strGlider		=  m_cGliderInfo.m_strModel;
		UpdateData(false);
		}
}
