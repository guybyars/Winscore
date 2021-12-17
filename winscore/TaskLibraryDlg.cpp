// TaskLibraryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "winscoredoc.h"
#include "TaskLibraryDlg.h"
#include "afxdialogex.h"
#include "global_prototypes.h"
#include "DirDialog.h"
#include "TaskDisplayDlg.h"

static int iSortColumn= 0;

// CTaskLibraryDlg dialog

IMPLEMENT_DYNAMIC(CTaskLibraryDlg, CResizingDialog)

CTaskLibraryDlg::CTaskLibraryDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CTaskLibraryDlg::IDD, pParent)
	, m_pDoc(NULL)
	, m_pTask(NULL)
{
    SetControlInfo(IDOK,                 ANCHORE_BOTTOM);
    SetControlInfo(IDC_SELECT,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_CANCEL,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_CHANGE_FOLDER,    ANCHORE_BOTTOM);
    SetControlInfo(IDC_VIEWTASK,         ANCHORE_BOTTOM);
    SetControlInfo(IDC_LISTCTRL,		RESIZE_HOR | RESIZE_VER);
}

CTaskLibraryDlg::~CTaskLibraryDlg()
{
}

void CTaskLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizingDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(CTaskLibraryDlg, CResizingDialog)
	ON_BN_CLICKED(IDC_SELECT, &CTaskLibraryDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_CANCEL, &CTaskLibraryDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHANGE_FOLDER, &CTaskLibraryDlg::OnBnClickedChangeFolder)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL, &CTaskLibraryDlg::OnNMDblclkListctrl)
	ON_BN_CLICKED(IDC_VIEWTASK, &CTaskLibraryDlg::OnBnClickedViewtask)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CTaskLibraryDlg::OnHdnItemclickListctrl)
END_MESSAGE_MAP()


// CTaskLibraryDlg message handlers


void CTaskLibraryDlg::OnBnClickedSelect()
{
	int iSel=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( iSel>=0 ) 
		{
		m_pTask=(CTask*)m_cListCtrl.GetItemData(iSel);
		}

	CResizingDialog::OnOK();

}

BOOL CTaskLibraryDlg::OnInitDialog() 
	{


	CResizingDialog::OnInitDialog();

	m_strLibraryPath=GetTaskLibraryPath();

	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

	LV_COLUMN lvc;
	int iCol=0;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;

	lvc.iSubItem = 0;
	lvc.pszText = _T("File Name");
	lvc.cx = 150;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Type");
	lvc.cx = 120;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	CString strText;
	lvc.pszText = _T("Min Dist");
	lvc.cx = 70;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	strText.Format("Distance (%s)",UnitsText(m_pDoc->m_eUnits));
	lvc.pszText = strText.GetBuffer();
	lvc.cx = 90;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Max Dist");
	lvc.cx = 70;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Time");
	lvc.cx = 70;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Turnpoints");
	lvc.cx = 150;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Start");
	lvc.cx = 120;
	m_cListCtrl.InsertColumn(iCol++,&lvc);

	m_cTaskList.LoadTaskLibrary(m_strLibraryPath);

	LoadTaskLibrary(m_strLibraryPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

	void CTaskLibraryDlg::OnBnClickedCancel()
	{
	CResizingDialog::OnCancel();
	}


	void CTaskLibraryDlg::OnBnClickedChangeFolder()
		{
		CDirDialog cdlg;
		cdlg.m_strSelDir=m_strLibraryPath;
		cdlg.m_strTitle="Task Library Folder";
		if( cdlg.DoBrowse() ==TRUE  ) 
			{
			m_strLibraryPath=cdlg.m_strPath;

			m_cTaskList.Purge();
			m_cTaskList.LoadTaskLibrary(cdlg.m_strPath);
			LoadTaskLibrary(cdlg.m_strPath);

			m_pTask=NULL;

			WriteWinscoreString( TASKPATH, m_strLibraryPath );
			}
		}

int CTaskLibraryDlg::LoadTaskLibrary(CString strLibraryPath)
	{
	int iItem=0;
	m_cListCtrl.DeleteAllItems();

	CString strWindowText;
	strWindowText.Format("Task Library - %s",strLibraryPath);
	SetWindowText(strWindowText);

	POSITION pos = m_cTaskList.GetHeadPosition();
	while(pos!=NULL)
		{
		CTask* pcTask=m_cTaskList.GetNext(pos);
		if( !pcTask ) continue;

		CTurnpoint *pcPoint=NULL;
		int nTurnpoints=pcTask->GetNumTurnpoints();
		for( int i=0; i<nTurnpoints; i++ )
			{
			pcPoint=(m_pDoc->m_turnpointArray.GetAt(pcTask->GetTurnpointID(i)-1));
			if( !pcPoint ) break;
			}
		if( !pcPoint ) continue;

		//Trap bogus tasks
		double dCheckDistance=pcTask->GetDistance(m_pDoc->m_turnpointArray,m_pDoc->m_eUnits);
		if( dCheckDistance<.01 || dCheckDistance>1000. ) continue;



		LV_ITEM lvi;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM;

		lvi.iItem = iItem;
		lvi.lParam = (LPARAM)pcTask;
		m_cListCtrl.InsertItem(&lvi);
		m_cListCtrl.SetItemText(iItem,0,pcTask->m_strFileName );

		m_cListCtrl.SetItemText(iItem,1,pcTask->TypeText() );

		CString strItem;
		if( pcTask->IsAreaTask() )
			{
			double dMin, dNominal, dMax;
			pcTask->GetTurnAreaDistances(	m_pDoc->m_turnpointArray, dMin, dNominal, dMax);
			pcTask->m_dColumnSortDistance[0]=dMin;
			pcTask->m_dColumnSortDistance[1]=dNominal;
			pcTask->m_dColumnSortDistance[2]=dMax;
      		strItem.Format( "%6.2lf",ConvertDistance(dMin,		SYSTEMUNITS, m_pDoc->m_eUnits));
			m_cListCtrl.SetItemText(iItem,2,strItem );
      		strItem.Format( "%6.2lf",ConvertDistance(dNominal,		SYSTEMUNITS, m_pDoc->m_eUnits));
			m_cListCtrl.SetItemText(iItem,3,strItem );
      		strItem.Format( "%6.2lf",ConvertDistance(dMax,		SYSTEMUNITS, m_pDoc->m_eUnits));

			m_cListCtrl.SetItemText(iItem,4,strItem );
			}
		else
			{
			m_cListCtrl.SetItemText(iItem,2,"" );
			strItem=pcTask->DistanceText(m_pDoc->m_turnpointArray,m_pDoc->m_eUnits);
			pcTask->m_dColumnSortDistance[0]=0.0;
			pcTask->m_dColumnSortDistance[1]=pcTask->GetDistance(m_pDoc->m_turnpointArray,m_pDoc->m_eUnits);
			pcTask->m_dColumnSortDistance[2]=0.0;
			m_cListCtrl.SetItemText(iItem,3,strItem );
			m_cListCtrl.SetItemText(iItem,4,"" );
			}


		m_cListCtrl.SetItemText(iItem, 5,pcTask->MinTimeText() );

		m_cListCtrl.SetItemText(iItem, 6,pcTask->TurnpointText() );

		m_cListCtrl.SetItemText(iItem, 7,pcTask->StartGateText(m_pDoc->m_turnpointArray) );

		iItem++;
		}

		return iItem;
	}


void CTaskLibraryDlg::OnNMDblclkListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int iSel=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( iSel>=0 )
		{
		m_pTask=(CTask*)m_cListCtrl.GetItemData(iSel);
		CResizingDialog::OnOK();
		}

}


void CTaskLibraryDlg::OnBnClickedViewtask()
{
	int iSel=m_cListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( iSel>=0 )
		{
		m_pTask=(CTask*)m_cListCtrl.GetItemData(iSel);
		
		if( m_pTask )
			{
			CTaskDisplayDlg dlg( m_pTask, m_pDoc );
			dlg.DoModal();
			}

		}

}


void CTaskLibraryDlg::OnHdnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if( abs(iSortColumn)==phdr->iItem+1 )
		{
		iSortColumn = -iSortColumn;
		}
	else
		iSortColumn = phdr->iItem+1;

	m_cTaskList.Sort(iSortColumn);
	LoadTaskLibrary(m_strLibraryPath);
}

void CTaskLibraryDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CTaskLibraryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
