// FlightSearchFoldersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "FlightSearchFoldersDlg.h"
#include "Global_Prototypes.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static CString strWSCPathFlag="<wscx file location>\\";
//static int     iWSCPathFlagLen=21;

/////////////////////////////////////////////////////////////////////////////
// CFlightSearchFoldersDlg dialog


CFlightSearchFoldersDlg::CFlightSearchFoldersDlg(CStringArray &strArray, CWnd* pParent /*=NULL*/)
	: CDialog(CFlightSearchFoldersDlg::IDD, pParent),
	m_strArray(strArray),m_pDoc(NULL)
{
	//{{AFX_DATA_INIT(CFlightSearchFoldersDlg)
	//}}AFX_DATA_INIT
	
}


void CFlightSearchFoldersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlightSearchFoldersDlg)
	DDX_Control(pDX, IDC_REMOVE, m_cDeleteButton);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ADD2, m_cRelativeButton);
	DDX_Control(pDX, IDC_ADD, m_cAddAbsolute);
	DDX_Control(pDX, IDC_CHECK1, m_cAutoDateCheckBox);
	DDX_Control(pDX, IDC_MOVEUP, m_cMoveUpButton);
}


BEGIN_MESSAGE_MAP(CFlightSearchFoldersDlg, CDialog)
	//{{AFX_MSG_MAP(CFlightSearchFoldersDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD2, &CFlightSearchFoldersDlg::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_CHECK1, &CFlightSearchFoldersDlg::OnBnAutoDateManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlightSearchFoldersDlg message handlers

void CFlightSearchFoldersDlg::OnOK() 
{
	m_bAutoDateCheck=m_cAutoDateCheckBox.GetCheck()==TRUE?true:false;

	if( !m_bAutoDateCheck )
		{
	m_strArray.RemoveAll();
	for( int i=0; i<m_cList.GetCount(); i++)
		{
		bool bdup=false;
		for( int j=0; j<m_strArray.GetSize(); j++)
			{
			CString strText;
			m_cList.GetText(i,strText);
			if( m_strArray[j]==strText ) bdup=true;
			}
		if( !bdup )
			{
			CString strText;
			m_cList.GetText(i,strText);
			m_strArray.Add(strText);
			}
		}	
		}

	m_cIGCDir.SetAutoDate(m_bAutoDateCheck);

	CDialog::OnOK();
}

void CFlightSearchFoldersDlg::OnAdd() 
{
	CDirDialog cdlg;
	cdlg.m_strSelDir=GetWSCPath();
	cdlg.m_strTitle="Path for flight logs";

	if( cdlg.DoBrowse() ==TRUE  ) 
		{

		bool bdup=false;
		for( int j=0; j<m_cList.GetCount(); j++)
			{
			CString strText;
			m_cList.GetText(j,strText);
			if( strText==cdlg.m_strPath ) bdup=true;
			}
		if( bdup )
			AfxMessageBox(_T("Folder is already in the list."));
		else
			{
			m_cList.InsertString(0,LPCSTR(cdlg.m_strPath));
			m_strDefault=cdlg.m_strPath;
			}
		}		
}

void CFlightSearchFoldersDlg::OnRemove() 
{
	int iSel = m_cList.GetCurSel();
	if( iSel<0) return;

	m_cList.DeleteString(iSel);
	m_cList.SetCurSel(iSel);

}

BOOL CFlightSearchFoldersDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cAutoDateCheckBox.SetCheck(m_cIGCDir.GetAutoDateFlag());

	m_cList.SetCurSel(0);

	m_cRelativeButton.EnableWindow(GetWSCPath().GetLength()>0);
	
	MasqueForm();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFlightSearchFoldersDlg::OnMoveup() 
{
	int iSel = m_cList.GetCurSel();
	if( iSel<0) return;

	CString strText;
	m_cList.GetText(iSel,strText);

	m_cList.DeleteString(iSel);	
	int iNew=max(iSel-1,0);
	m_cList.InsertString(iNew, strText);
	m_cList.SetCurSel(iNew);
}

void CFlightSearchFoldersDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	
}


void CFlightSearchFoldersDlg::OnBnClickedAdd2()
	{
	CDirDialog cdlg;
	cdlg.m_strSelDir=GetWSCPath();
	cdlg.m_strTitle="Path for flight logs";

	if( cdlg.DoBrowse() ==TRUE  ) 
		{

		bool bdup=false;
		for( int j=0; j<m_cList.GetCount(); j++)
			{
			CString strText;
			m_cList.GetText(j,strText);
			if( strText==cdlg.m_strPath ) bdup=true;
			}
		if( bdup )
			AfxMessageBox(_T("Folder is already in the list."));
		else
			{
			m_cList.InsertString(0,LPCSTR(GetRelativePath(cdlg.m_strPath)));
			m_strDefault=cdlg.m_strPath;
			}
		}		
	}


void CFlightSearchFoldersDlg::OnBnAutoDateManage()
	{
	MasqueForm();
	}
	
void CFlightSearchFoldersDlg::MasqueForm()
	{
	if( m_cAutoDateCheckBox.GetCheck() )
		{
		CStringArray straDateFolders;
		// Populate List with automatic info.
		 m_cRelativeButton.EnableWindow(FALSE);
		 m_cAddAbsolute.EnableWindow(FALSE);
		 m_cMoveUpButton.EnableWindow(FALSE);
		 m_cDeleteButton.EnableWindow(FALSE);
		 m_cList.EnableWindow(FALSE);
		 m_cIGCDir.GetAutoDateFolders(m_pDoc, straDateFolders);
		// Populate List with auto info
		m_cList.ResetContent();
		for( int i=0; i<straDateFolders.GetSize(); i++)
			{
			if( i==0 ) m_strDefault=straDateFolders[i];
			m_cList.AddString(LPCSTR(straDateFolders[i]));
			}

		}
	else
		{
		 m_cRelativeButton.EnableWindow(TRUE);
		 m_cAddAbsolute.EnableWindow(TRUE);
		 m_cMoveUpButton.EnableWindow(TRUE);
		 m_cDeleteButton.EnableWindow(TRUE);
		 m_cList.EnableWindow(TRUE);
		// Populate List with user entered. info.
		m_cList.ResetContent();
		for( int i=0; i<m_strArray.GetSize(); i++)
			{
			if( i==0 ) m_strDefault=m_strArray[i];
			m_cList.AddString(LPCSTR(m_strArray[i]));
			}
		}

	}