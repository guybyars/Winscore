// HandicapCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "HandicapCalcDlg.h"
#include "accombobox.h"
#include "Global_Prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandicapCalcDlg dialog


CHandicapCalcDlg::CHandicapCalcDlg(CGliderInfoList &cGIList, CGliderInfo *pcGliderInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CHandicapCalcDlg::IDD, pParent),
	m_cGIList(cGIList),
	m_pcGliderInfo(pcGliderInfo)

{
	//{{AFX_DATA_INIT(CHandicapCalcDlg)
	m_fSpan = 0.0f;
	m_fWeight = 0.0f;
	m_strNotes = _T("");
	m_strSpan = _T("");
	m_strWeight = _T("");
	m_strManufacturer = _T("");
	m_dHandicap = 0.0;
	m_bWingletCheck = FALSE;
	m_bTurbulationCheck = FALSE;
	m_bRootFaringCheck = FALSE;
	//}}AFX_DATA_INIT
	m_bInitDialog=false;
	}


void CHandicapCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHandicapCalcDlg)
	DDX_Control(pDX, IDC_ROOT_FAIRING_CHECK, m_cRootFaringCheckBox);
	DDX_Control(pDX, IDC_TURBULATION_CHECK, m_cTurbulationCheckBox);
	DDX_Control(pDX, IDC_WINGLET_CHECK, m_cWingletCheckBox);
	DDX_Control(pDX, IDC_MANUFACTURER_COMBO, m_cManufacturerCombo);
	DDX_Control(pDX, IDC_HANDICAP_EDIT, m_cHandicapEdit);
	DDX_Control(pDX, IDC_SPAN_EDIT, m_cSpanEdit);
	DDX_Control(pDX, IDC_WEIGHT_EDIT, m_cWeightEdit);
	DDX_Control(pDX, IDC_MODEL_COMBO, m_cModel);
	DDX_Text(pDX, IDC_SPAN_EDIT, m_fSpan);
	DDV_MinMaxFloat(pDX, m_fSpan, 0.f, 40.f);
	DDX_Text(pDX, IDC_WEIGHT_EDIT, m_fWeight);
	DDV_MinMaxFloat(pDX, m_fWeight, 0.f, 5000.f);
	DDX_Text(pDX, IDC_NOTES_TEXT, m_strNotes);
	DDX_Text(pDX, IDC_SPAN_TEXT, m_strSpan);
	DDX_Text(pDX, IDC_WEIGHT_TEXT, m_strWeight);
	DDX_Text(pDX, IDC_HANDICAP_EDIT, m_dHandicap);
	DDV_MinMaxDouble(pDX, m_dHandicap, 0., 3.);
	DDX_Check(pDX, IDC_WINGLET_CHECK, m_bWingletCheck);
	DDX_Check(pDX, IDC_TURBULATION_CHECK, m_bTurbulationCheck);
	DDX_Check(pDX, IDC_ROOT_FAIRING_CHECK, m_bRootFaringCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHandicapCalcDlg, CDialog)
	//{{AFX_MSG_MAP(CHandicapCalcDlg)
	ON_CBN_SELCHANGE(IDC_MODEL_COMBO, OnSelchangeModelCombo)
	ON_EN_UPDATE(IDC_SPAN_EDIT, OnUpdateSpanEdit)
	ON_EN_UPDATE(IDC_WEIGHT_EDIT, OnUpdateWeightEdit)
	ON_CBN_SELCHANGE(IDC_MANUFACTURER_COMBO, OnSelchangeManufacturerCombo)
	ON_BN_CLICKED(IDC_TURBULATION_CHECK, OnTurbulationCheck)
	ON_BN_CLICKED(IDC_WINGLET_CHECK, OnWingletCheck)
	ON_BN_CLICKED(IDC_ROOT_FAIRING_CHECK, OnRootFairingCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandicapCalcDlg message handlers

void CHandicapCalcDlg::OnOK() 
{
	UpdateData(true);

	int iSel=m_cModel.GetCurSel();
	if( iSel>=0 ) 
		m_pcGliderInfo=(CGliderInfo*)m_cModel.GetItemDataPtr(iSel);
	else
		m_pcGliderInfo=NULL;
	
	CDialog::OnOK();
}

void CHandicapCalcDlg::OnSelchangeModelCombo() 
{
	CString strModel;
	CGliderInfo *pcInfo=NULL;
	int iSel=m_cModel.GetCurSel();
	if( iSel>=0 ) 
		pcInfo=(CGliderInfo*)m_cModel.GetItemDataPtr(iSel);

	if( pcInfo )
		{
		if( !m_bInitDialog || m_fSpan==0 )
			{
			m_dHandicap	=pcInfo->m_dHandicap;
			m_fSpan		=pcInfo->m_fSpan;
			m_fWeight	=pcInfo->m_fWeight;
			}
		m_strWeight.Format(_T("%5.0lf lb"), pcInfo->m_fWeight);
		m_strSpan.Format(_T("%5.1lf m"), pcInfo->m_fSpan);

		m_strNotes=_T("");
		if( pcInfo->m_strNotes.Find(_T("S"))>=0 ) 
			m_strNotes+=_T("Standard Class  ");
		if( pcInfo->m_strNotes.Find(_T("M"))>=0 ) 
			m_strNotes+=_T("Motorized  ");
		if( pcInfo->m_strNotes.Find(_T("F"))>=0 ) 
			m_strNotes+=_T("Fixed Gear  ");
		if( pcInfo->m_strNotes.Find(_T("R"))>=0 ) 
			m_strNotes+=_T("Retractable Gear ");
		if( pcInfo->m_strNotes.Find(_T("W"))>=0 ) 
			m_strNotes+=_T("Factory Winglets ");
		}
	else
		{
		m_dHandicap=1.0;
		}
	UpdateData(false);

}

BOOL CHandicapCalcDlg::OnInitDialog() 
{
	m_bInitDialog=true;
	CDialog::OnInitDialog();
	
	CStringArray cStrArray;
	m_cGIList.GetManufacturers(cStrArray);
	for( int i=0; i<cStrArray.GetSize(); i++)
		{
		m_cManufacturerCombo.AddString(cStrArray[i]);
		}
	if( m_pcGliderInfo )
		{
		m_cManufacturerCombo.SelectString(0,m_pcGliderInfo->m_strManufacturer);
		OnSelchangeManufacturerCombo();
		//m_cModel.SelectString(-1,m_pcGliderInfo->m_strModel);
		for( int i=0; i<m_cModel.GetCount(); i++)
			{
			CGliderInfo* pcGliderInfo=(CGliderInfo*)m_cModel.GetItemDataPtr(i);
			if( m_pcGliderInfo->m_strModel==pcGliderInfo->m_strModel &&
				m_pcGliderInfo->m_strNotes==pcGliderInfo->m_strNotes	)
				{
				m_cModel.SetCurSel(i);
				break;
				}
			}
		}
	else
		{
		m_cManufacturerCombo.SetCurSel(0);
		OnSelchangeManufacturerCombo();
		m_cModel.SetCurSel(0);
		}

	OnSelchangeModelCombo();
	RecalculateHandicap();	

	UpdateData(false);
	m_bInitDialog=false;

	CString strTitle=_T("Hdcap Calc - ")+m_strTitle;
	SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHandicapCalcDlg::OnUpdateSpanEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	RecalculateHandicap();	
	
}

void CHandicapCalcDlg::OnUpdateWeightEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	RecalculateHandicap();	
}

void CHandicapCalcDlg::RecalculateHandicap()
{
	CString strSpan;
	m_cSpanEdit.GetWindowText(strSpan);
	m_fSpan=(float)atof(strSpan);

	CString strWeight;
	m_cWeightEdit.GetWindowText(strWeight);
	m_fWeight=(float)atof(strWeight);

	int iSel=m_cModel.GetCurSel();
	CGliderInfo *pcInfo=(CGliderInfo*)m_cModel.GetItemDataPtr(iSel);
	if( pcInfo==NULL ) 
		{
	    m_cHandicapEdit.SetWindowText("1.0");
		return;
		}

	m_dHandicap=GetAdjustedHandicap(	pcInfo, 
										m_fWeight, 
										m_fSpan, 
										m_cRootFaringCheckBox.GetCheck(), 
										m_cTurbulationCheckBox.GetCheck(), 
										m_cWingletCheckBox.GetCheck() );

	CString strHandicap;
	strHandicap.Format(_T("%5.4lf"),m_dHandicap);
	m_cHandicapEdit.SetWindowText(strHandicap);

}

void CHandicapCalcDlg::OnSelchangeManufacturerCombo() 
{
	int iSel=m_cManufacturerCombo.GetCurSel();

	m_cManufacturerCombo.GetLBText( iSel, m_strManufacturer );
	m_cModel.ResetContent();
	CPtrArray cPtrArray;
	m_cGIList.GetModels(m_strManufacturer, cPtrArray);
	CGliderInfo *pNextInfo=NULL, *pPrevInfo=NULL;
	for( int i=0; i<cPtrArray.GetSize(); i++)
		{
		CString strAppend;
		CGliderInfo *pInfo=(CGliderInfo*)cPtrArray[i];
		if( i<cPtrArray.GetSize()-1 ) 
			{
			//Check the next entry, if it is the same and this one has winglets, mark it.
			pNextInfo=(CGliderInfo*)cPtrArray[i+1];
			if( pInfo->m_strModel==pNextInfo->m_strModel )
				{
				// Duplicate models
				if( pInfo->m_strNotes.Find("W")>=0 && pNextInfo->m_strNotes.Find("W")<0 )
					strAppend=" (Factory Winglets)";
				}
			}
		if( i>0 && strAppend.GetLength()==0 ) 
			{
			//Check the previous entry, if it is the same and this one has winglets, mark it.
			pPrevInfo=(CGliderInfo*)cPtrArray[i-1];
			if( pInfo->m_strModel==pPrevInfo->m_strModel )
				{
				// Duplicate models
				if( pInfo->m_strNotes.Find("W")>=0  && pPrevInfo->m_strNotes.Find("W")<0 )
					strAppend=" (Factory Winglets)";
				if( pPrevInfo->m_fSpan != pInfo->m_fSpan )
					{
					CString strSpan;
					strSpan.Format(" - %3.1f",pInfo->m_fSpan);
					strAppend+=strSpan;
					}
				}
			}
		int iIndex=m_cModel.AddString(pInfo->m_strModel+strAppend);
		m_cModel.SetItemDataPtr(iIndex,pInfo);
		}	
	m_cModel.SetCurSel(0);
}

void CHandicapCalcDlg::OnTurbulationCheck() 
{
	RecalculateHandicap();	
	
}

void CHandicapCalcDlg::OnWingletCheck() 
{
	RecalculateHandicap();	
	
}

void CHandicapCalcDlg::OnRootFairingCheck() 
{
	RecalculateHandicap();	
	
}
