//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// CalculateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "CalculateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalculateDlg dialog


CCalculateDlg::CCalculateDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CCalculateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalculateDlg)
	//}}AFX_DATA_INIT

 
    SetControlInfo(IDC_CLASS_TAB,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDC_STATUS_LIST,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDC_DATE_COMBO,    RESIZE_HOR );
    SetControlInfo(IDCANCEL,    ANCHORE_BOTTOM );
    SetControlInfo(IDC_COPY,    ANCHORE_BOTTOM );


}


void CCalculateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalculateDlg)
	DDX_Control(pDX, IDC_STATUS_LIST, m_cStatusListCtrl);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
	DDX_Control(pDX, IDC_CLASS_TAB, m_cClassTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalculateDlg, CDialog)
	//{{AFX_MSG_MAP(CCalculateDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CLASS_TAB, OnSelchangeClassTab)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO, OnSelchangeDateCombo)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalculateDlg message handlers

void CCalculateDlg::Calculate() 
{

	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	EClass eClass=m_cClassTab.GetCurClass();

	m_cStatusListCtrl.DeleteAllItems();
	m_pDoc->CalculateScores( *pcTime, eClass, m_cStatusListCtrl );

}

BOOL CCalculateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc->SetAvailableClasses( m_cClassTab );
//	m_pDoc->LoadDateTabs(m_cClassTab );
		
	m_pDoc->LoadDateComboBox(m_cDateCombo);
	CreateStatusColumns();

    Calculate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



#define NUM_COLUMNS 2

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("*"), _T("Status")
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	150, 1000
};

void CCalculateDlg::CreateStatusColumns()
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		m_cStatusListCtrl.InsertColumn(i,&lvc);
	}

}


void CCalculateDlg::OnSelchangeClassTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	BOOL flag=m_cStatusListCtrl.DeleteAllItems();
    ASSERT(flag);
	Calculate();
	*pResult = 0;
}

void CCalculateDlg::OnSelchangeDateCombo() 
{
	Calculate();	
}

void CCalculateDlg::OnCancel() 
{
	int iSel=m_cDateCombo.GetCurSel();
	ASSERT(iSel>=0);
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	m_cDate=*pcTime;
	m_eClass=m_cClassTab.GetCurClass();

	CDialog::OnCancel();
}

void CCalculateDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CCalculateDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

static const char TAB = '\t';

void CCalculateDlg::OnCopy() 
{
	CString str;

	for( int i=0; i<m_cStatusListCtrl.GetItemCount(); i++ )
		{
		str+=m_cStatusListCtrl.GetItemText( i, 0);
		str+=TAB;
		str+=m_cStatusListCtrl.GetItemText( i, 1);
		str+=_T("\r\n");
		}

    HGLOBAL hglobalText = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, str.GetLength() + 1);
    strcpy_s((TCHAR *)GlobalLock(hglobalText), str.GetLength()+1, str);
    GlobalUnlock(hglobalText);
    
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,hglobalText);
    CloseClipboard();

	
}
