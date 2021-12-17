//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ExportScoresheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "ExportScoresheetDlg.h"
#include "ExportSuccessDlg.h"
#include "DescriptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int iDefault=0;

/////////////////////////////////////////////////////////////////////////////
// CExportScoresheetDlg dialog


CExportScoresheetDlg::CExportScoresheetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportScoresheetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportScoresheetDlg)
	m_cFileName = _T("");
	//}}AFX_DATA_INIT
}


void CExportScoresheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportScoresheetDlg)
	DDX_Control(pDX, IDC_CLASS_LIST, m_cClassListCtrl);
	DDX_Control(pDX, IDC_FORMAT_COMBO, m_cFormatCombo);
	DDX_Control(pDX, IDC_DAY_RADIO, m_cDayRadio);
	DDX_Control(pDX, IDC_CUM_RADIO, m_cCumRadio);
	DDX_Control(pDX, IDC_DATE_COMBO2, m_cDateCombo);
	DDX_Text(pDX, IDC_FILE_EDIT, m_cFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportScoresheetDlg, CDialog)
	//{{AFX_MSG_MAP(CExportScoresheetDlg)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseButton)
	ON_BN_CLICKED(IDC_CUM_RADIO, OnCumRadio)
	ON_BN_CLICKED(IDC_DAY_RADIO, OnDayRadio)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO2, OnSelchangeDateCombo2)
	ON_CBN_SELCHANGE(IDC_FORMAT_COMBO, OnSelchangeFormatCombo)
	ON_BN_CLICKED(IDC_COMMENT, OnComment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportScoresheetDlg message handlers

void CExportScoresheetDlg::OnBrowseButton() 
	{
	CString cSuffix;

	UpdateData();

	if( m_cFormatCombo.GetCurSel()==0 )
		cSuffix="xml";
	else
		cSuffix="txt";

	CFileDialog  cFileDlg(FALSE, cSuffix, m_cFileName, OFN_OVERWRITEPROMPT);

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );
	cFileDlg.m_ofn.lpstrInitialDir=strPath;

	if( cFileDlg.DoModal()==IDOK )
		{
		m_cFileName=cFileDlg.GetPathName();
		UpdateData(FALSE);
		}
	}

void CExportScoresheetDlg::OnCumRadio() 
{
	m_cDayRadio.SetCheck(0);
    m_cCumRadio.SetCheck(1);
	SetDefaultFileName();		
}

void CExportScoresheetDlg::OnDayRadio() 
{
	m_cDayRadio.SetCheck(1);
    m_cCumRadio.SetCheck(0);
	SetDefaultFileName();	
}


BOOL CExportScoresheetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cDayRadio.SetCheck(1);
    m_cCumRadio.SetCheck(0);

	m_pDoc->LoadDateComboBox(m_cDateCombo);
	m_cFormatCombo.SetCurSel((int)(eText)-1);

	m_cClassListCtrl.AddColumn("Classes to Export");
	m_cClassListCtrl.SetAvailableClasses();

	GreyRadioButtons(FALSE);

	m_cFormatCombo.SetCurSel(iDefault);
	iDefault==0?m_eExportType=eText:m_eExportType=eXML;

	SetDefaultFileName();
	m_cComment="";

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportScoresheetDlg::OnOK() 
	{
	CStdioFile  cFile;
	CWinscoreListCtrl  cDum;
	EScoresheetType eType;
	CString strFail;

	cDum.m_fReport=FALSE;
	if( m_cFormatCombo.GetCurSel()==0 ) 
		m_eExportType=eText;
	else
		m_eExportType=eXML;
	iDefault=m_cFormatCombo.GetCurSel();
	
	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcDate=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	UpdateData();

	if( m_cDayRadio.GetCheck() )
		eType=eDay;
	else
		eType=eSummary;

	int nClasses=m_cClassListCtrl.GetNumCheckedClasses();
	if( nClasses==0 ) 
		{
		AfxMessageBox(_T("No classes were selected"));
		return;
		}

	CFileStatus rStatus;
	if( CFile::GetStatus( m_cFileName, rStatus ) )
		{
		if( AfxMessageBox( m_cFileName+_T(" already exists, Overwrite?"), MB_ICONQUESTION  | MB_YESNO )==IDNO ) return;
		}

	if( m_eExportType==eXML ) 
		{

		EClass m_aeClasses[NUMCLASSES];
		for( int iClass=0; iClass< nClasses; iClass++ )
			{
			m_aeClasses[iClass]=m_cClassListCtrl.GetCheckedClass(iClass);
			}
		if( eType==eSummary )
			{
			bool bret=m_pDoc->ExportCumulativeXML( m_cFileName, nClasses, m_aeClasses );
			if(!bret )
				{
				AfxMessageBox(_T("Error writing file: ") + m_cFileName, MB_ICONSTOP);
				return;
				}
			}
		else if( eType==eDay )
			{
			bool bret=m_pDoc->ExportDayXML( m_cFileName, nClasses, m_aeClasses, *pcDate, strFail );
			if(!bret )
				{
				AfxMessageBox(_T("Error writing file: ") + m_cFileName, MB_ICONSTOP);
				return;
				}
			}
		CFileException e;
		if( cFile.Open( m_cFileName, CFile::modeRead, &e ) )
			{
			CString strPath=cFile.GetFilePath();
			strPath.Replace(LPCTSTR(cFile.GetFileName()), _T(""));
			AfxGetApp()->WriteProfileString( REGSECTION, EXPORTPATH, strPath );
			cFile.Close();
			}

		}
	else
		{
		CFileException e;
		if( !cFile.Open( m_cFileName, CFile::modeCreate | CFile::modeWrite, &e ) )
			{
			AfxMessageBox(_T("Could not open file: ") + m_cFileName);
			return;
			}

		CString strPath=cFile.GetFilePath();
		strPath.Replace(LPCTSTR(cFile.GetFileName()), _T(""));
		AfxGetApp()->WriteProfileString( REGSECTION, EXPORTPATH, strPath );

		ExportHeader(cFile, *pcDate, m_eExportType, eType);

		for( int iClass=0; iClass< nClasses; iClass++ )
			{
			ExportScoresheet(cFile,	m_cClassListCtrl.GetCheckedClass(iClass), *pcDate, m_eExportType, eType );
			}

		ExportTrailer(cFile, m_eExportType );

		cFile.Close();
		}

	AfxMessageBox( m_cFileName+_T(" successfully written"), MB_ICONINFORMATION   );


//	CExportSuccessDlg cSuccessDlg;
//	cSuccessDlg.DoModal();

	CDialog::OnOK();
	}

void CExportScoresheetDlg::OnSelchangeDateCombo2() 
{
SetDefaultFileName();	
}

void CExportScoresheetDlg::SetDefaultFileName()
{
	CString cRootName;
	CString cDateString;

	cRootName=_T("Scoresheet");

	if( m_cDayRadio.GetCheck() )
		{
		int iSel=m_cDateCombo.GetCurSel();
		CTime *pcDate=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
		cDateString= pcDate->Format(_T("%b%d_%y"));
		m_cFileName=cRootName+cDateString;
		}
	else
		{
		if( m_eExportType==eText )
			m_cFileName=_T("ContestSummary");
		else
			m_cFileName=_T("SSAContestSummary");
		}

	if( m_eExportType==eXML)
		m_cFileName+=_T(".xml");
	else
		m_cFileName+=_T(".txt");

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );

	if( strPath.GetLength()>0 )
		m_cFileName=strPath+m_cFileName;

	UpdateData(FALSE);
}

void CExportScoresheetDlg::OnSelchangeFormatCombo() 
{
	if( m_cFormatCombo.GetCurSel()==0 ) 
		m_eExportType=eText;
	else
		m_eExportType=eXML;
	iDefault=m_cFormatCombo.GetCurSel();

	SetDefaultFileName();
}

void CExportScoresheetDlg::GreyRadioButtons(BOOL fGrey)
{
	m_cDayRadio.EnableWindow(!fGrey);	
	m_cCumRadio.EnableWindow(!fGrey);	

}

void  CExportScoresheetDlg::ExportScoresheet(	CStdioFile		&cFile,
												EClass			eClass, 
												CTime			&cDate, 
												EExportType		eExportType, 
												EScoresheetType eType)
	{


	CWinscoreListCtrl  cDum;
	CString cLine;
	CString cNEWLINE=_T("\n");

	if( eExportType==eText )
		{
		if( eType==eSummary )
			{
			m_pDoc->ExportCumulativeText( cFile, eClass,  cNEWLINE );
			}
		else if( eType==eDay )
			{
			m_pDoc->ExportDayText(	cFile, eClass, cDate, eCumulative, cNEWLINE);
			}
		}
	}


void  CExportScoresheetDlg::ExportHeader(	CStdioFile		 &cFile,
											CTime			&cDate, 
											EExportType		eExportType, 
											EScoresheetType eType)
	{
	if( eExportType==eText )
		{	
		CString cNEWLINE=_T("\n");
		m_pDoc->ExportTextHeader( cFile, cDate, eType, cNEWLINE);
		}
	}

void  CExportScoresheetDlg::ExportTrailer(	CStdioFile		 &cFile,
											EExportType		eExportType )
	{	

	if( eExportType==eText )
		{
		CString cNEWLINE=_T("\n");
		m_pDoc->ExportTextTrailer(cFile, cNEWLINE, m_cComment , eText );
		}
	}

void CExportScoresheetDlg::OnComment() 
{
	CDescriptionDlg   cDlg;
	cDlg.m_Text=m_cComment;
	cDlg.m_cTitle=_T("Comment");
	if (cDlg.DoModal() == IDOK)
		{
		m_cComment=cDlg.m_Text;
		}
	
}

