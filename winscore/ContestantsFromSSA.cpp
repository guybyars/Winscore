// ContestantsFromSSA.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ContestantsFromSSA.h"
#include "global_prototypes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContestantsFromSSA dialog

CContestantsFromSSA::CContestantsFromSSA(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CContestantsFromSSA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContestantsFromSSA)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    SetControlInfo(IDOK,                 ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,             ANCHORE_BOTTOM);
    SetControlInfo(IDC_LOOKUP_BUTTON,    ANCHORE_BOTTOM); 
	SetControlInfo(IDC_CONTROL_BOX,    RESIZE_HOR | RESIZE_VER);


}


void CContestantsFromSSA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContestantsFromSSA)
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDC_LOOKUP_BUTTON, m_cLookupButton);
	DDX_Control(pDX, IDC_COMBO1, m_cContestCombo);
	DDX_Control(pDX, IDC_CONTROL_BOX, m_cCListCtrlBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContestantsFromSSA, CDialog)
	//{{AFX_MSG_MAP(CContestantsFromSSA)
	ON_BN_CLICKED(IDC_LOOKUP_BUTTON, OnLookupButton)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CContestantsFromSSA::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContestantsFromSSA message handlers

void CContestantsFromSSA::OnLookupButton() 
{
	int iSSA=0;

	int iIndex=m_cContestCombo.GetCurSel();
	if( iIndex>=0 ) 
		{
		iSSA=m_cContestCombo.GetItemData(iIndex);
		}

	if( iSSA==0 ) 
		{
		AfxMessageBox(_T("Invalid Contest"));
		return;
		}

	CString strURL;
//	strURL.Format(_T("https://www.ssa.org/cde/contestant_xml.asp?ContestId=%d"),iSSA);
	strURL.Format("https://members.ssa.org/cde/winscore_xml.asp?ContestId=%d",iSSA);
	try
		{
		CWaitCursor cWait;
		CXMLMgr cMgr;
		cMgr.Initlize();

		TESTHR(cMgr.Load(strURL) );

		CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
//		if(strRoot!=_T("SSAContest") )
		if(strRoot!="Winscore" )
			{
			CString strWarn;
			strWarn.Format("Contest information at ssa.org is not valid for this contest.\n\n %s",strURL);
			AfxMessageBox( strWarn,  MB_ICONSTOP   );
			return;
			}

		MSXML2::IXMLDOMNodePtr pContestants;
		TESTHR(cMgr.SelectChild(_T("contestants"), cMgr.GetRoot(), pContestants));
		if( pContestants==NULL )
			{
			AfxMessageBox( _T("No contestants found at ssa.org for this contest."),  MB_ICONSTOP   );
			return;
			}

		int nSkipped=0;
		m_cContestantList.Purge();
		int nImported=m_cContestantList.ImportSSA(cMgr,cMgr.GetRoot(), m_pDoc->m_contestantList, nSkipped);

		m_cContestantList.CheckClasses(m_pDoc->m_eContest);

		// Set the default handicap from the handicap file
		m_cContestantList.UpdateHandicaps(m_pDoc->m_eContest, m_pDoc->m_cGIList);

		m_cContestantList.LoadContestantList( m_cCListCtrlBox );

		CString strStatus;
		if( nSkipped==0 ) 
			strStatus.Format( _T("%d contestants read from ssa.org"), nImported);
		else
			strStatus.Format( _T("%d new contestants read from ssa.org. %d contestant(s) were either already entered, or duplicate CIDs were imported from ssa.org."), nImported, nSkipped);
		AfxMessageBox( strStatus,  MB_ICONINFORMATION      );

		m_cOK.EnableWindow(nImported>0);
		}
	catch (...)
		{
		AfxMessageBox( _T("Unexpected error reading contestant information from members.ssa.org."),  MB_ICONSTOP   );
		}
	
}

void CContestantsFromSSA::OnOK() 
{

	int iIndex=m_cContestCombo.GetCurSel();
	if( iIndex>=0 ) 
		{
		int iSSA=m_cContestCombo.GetItemData(iIndex);
		if( iSSA>0 ) 
			m_pDoc->m_iSSA_ID=iSSA;
		}

	CDialog::OnOK();
}

BOOL CContestantsFromSSA::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWaitCursor cWait;
	
	m_cCListCtrlBox.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	m_pDoc->m_contestantList.CreateControlColumns(m_cCListCtrlBox);

	int nLoaded=LoadContestsFromSSA( &m_cContestCombo, NULL ); 

	if( nLoaded==0 )
		{
		AfxMessageBox(_T("Unable to read contest information from ssa.org.  Make sure you are connected to the Internet."));
		}
	
	for( int i=0; i<m_cContestCombo.GetCount(); i++)
		{
		if( m_pDoc->m_iSSA_ID==(int)m_cContestCombo.GetItemData(i) )
			{
			m_cContestCombo.SetCurSel(i);
			}

		}

	m_cOK.EnableWindow(false);

	m_cLookupButton.EnableWindow(m_cContestCombo.GetCount()>0 && m_cContestCombo.GetCurSel()>=0);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContestantsFromSSA::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CContestantsFromSSA::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CContestantsFromSSA::OnSelchangeCombo1() 
{
	m_cLookupButton.EnableWindow(m_cContestCombo.GetCount()>0 && m_cContestCombo.GetCurSel()>=0);

}


void CContestantsFromSSA::OnBnClickedCancel()
{
	CResizingDialog::OnCancel();
}
