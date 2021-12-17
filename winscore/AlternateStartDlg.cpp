// AlternateStartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "AlternateStartDlg.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlternateStartDlg dialog


CAlternateStartDlg::CAlternateStartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlternateStartDlg::IDD, pParent)
	, m_pDoc(NULL)
{
	//{{AFX_DATA_INIT(CAlternateStartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_cStartTime=0;
	m_nMaxRet=15;
	m_bHandicapped=false;
}


void CAlternateStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlternateStartDlg)
	DDX_Control(pDX, IDC_LIST, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlternateStartDlg, CDialog)
	//{{AFX_MSG_MAP(CAlternateStartDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlternateStartDlg message handlers

BOOL CAlternateStartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	TCHAR		buff[10];
	CString strSpeedText;
	int			iCurrentStart=0;

	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_CENTER;

	lvc.iSubItem = 0;
	lvc.pszText = _T("Start Time");
	lvc.cx = 70;
	m_cListCtrl.InsertColumn(0,&lvc);

	lvc.iSubItem = 1;
	lvc.pszText = _T("Scored Time on Course");
	lvc.cx =  130;
	m_cListCtrl.InsertColumn(1,&lvc);

	lvc.iSubItem = 2;
	CString strItem;
	if( m_bHandicapped ) 
		strItem.Format("Hdcap Speed %s",UnitsSpeedText(m_pDoc->m_eUnits));
	else
		strItem.Format("Speed %s",UnitsSpeedText(m_pDoc->m_eUnits));
	lvc.pszText =strItem.GetBuffer();
	lvc.cx =  95;
	m_cListCtrl.InsertColumn(2,&lvc);

	lvc.iSubItem = 3;
	if( m_bHandicapped )
		strItem.Format("Hdcap Dist (%s)",UnitsText(m_pDoc->m_eUnits));
	else
		strItem.Format("Distance (%s)",UnitsText(m_pDoc->m_eUnits));
	lvc.pszText =strItem.GetBuffer();
	lvc.cx =  105;
	m_cListCtrl.InsertColumn(3,&lvc);


	lvc.iSubItem = 4;
	lvc.pszText = _T("Est. Score Change");
	lvc.cx =  130;
	m_cListCtrl.InsertColumn(4,&lvc);

	lvc.iSubItem = 5;
	lvc.pszText = _T("Start Penalty");
	lvc.cx =  80;
	m_cListCtrl.InsertColumn(5,&lvc);

	lvc.iSubItem = 6;
	lvc.pszText = _T("Estimated Score");
	lvc.cx =  100;
	m_cListCtrl.InsertColumn(6,&lvc);

	for( int iItem=0; iItem<m_nStarts; iItem++ )
		{
		LV_ITEM lvi;

		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM;

		lvi.iItem = iItem;
		lvi.lParam = (LPARAM)m_caStartTimes[iItem].GetTime();
		m_cListCtrl.InsertItem(&lvi);
		m_cListCtrl.SetItemText(iItem,0,m_caStartTimes[iItem].Format(_T("%H : %M : %S")) );  // Start Time

		if( m_caTOCs[iItem]>0 ) 
			m_cListCtrl.SetItemText(iItem,1,m_caTOCs[iItem].Format(_T(" %H:%M:%S ") ) ); // Time on course
		else
			m_cListCtrl.SetItemText(iItem,1,""); // Time on course

		if( m_daSpeeds[iItem]>0.0 && m_caTOCs[iItem]>0 ) 
			{
			strSpeedText.Format(_T("%5.2lf"), ConvertDistance(m_daSpeeds[iItem], SYSTEMUNITS, m_pDoc->m_eUnits ) );
			m_cListCtrl.SetItemText(iItem,2,strSpeedText ); // Speed
			}
		else
			m_cListCtrl.SetItemText(iItem,2,""); // Speed

		strSpeedText.Format(_T("%5.2lf"), ConvertDistance(m_daDistances[iItem], SYSTEMUNITS, m_pDoc->m_eUnits ) );
		m_cListCtrl.SetItemText(iItem,3,strSpeedText ); // Distance

		_itot_s( m_iaPointDiff[iItem], buff, 10,10);
		m_cListCtrl.SetItemText(iItem,4,buff );  //Est. Score Reduction

		_itot_s( m_iaPenalties[iItem], buff, 10,10);
		m_cListCtrl.SetItemText(iItem,5,buff ); // Penalty Points

		_itot_s( m_iaScore[iItem], buff, 10,10);
		CString strItem(buff);
//		if( iItem==iBestScorePtr ) strItem+="<";
		m_cListCtrl.SetItemText(iItem,6,strItem ); // Estimated Score

		}

	m_cListCtrl.SetItemState(m_iBestScorePtr, LVIS_SELECTED, LVIS_SELECTED);//LVIS_SELECTED

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlternateStartDlg::OnOK() 
{
	m_cStartTime=0;
	int iItem=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( iItem>=0 ) 
		m_cStartTime=(int)m_cListCtrl.GetItemData( iItem );
	
	CDialog::OnOK();
}



void CAlternateStartDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CAlternateStartDlg::OnOK();
	
	*pResult = 0;
}
