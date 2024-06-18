//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ExportFlightFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ExportFlightFileDlg.h"
#include "FlightLogTransferSettings.h"
#include "global_Prototypes.h"
#include <Atlbase.h>
#include "ZipArchive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportFlightFileDlg dialog


CExportFlightFileDlg::CExportFlightFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportFlightFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportFlightFileDlg)
	m_strFileName = _T("");
	m_strStatus = _T("");

	//}}AFX_DATA_INIT
}


void CExportFlightFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportFlightFileDlg)
//	DDX_Control(pDX, IDC_VIEW_BUTTON, m_cViewButton);
	DDX_Control(pDX, IDOK, m_cExportButton);
	DDX_Control(pDX, IDCANCEL, m_cCancelButton);
	DDX_Control(pDX, IDC_PROGRESS, m_cProgress);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
	DDX_Control(pDX, IDC_CLASS_LIST, m_cClassList);
	DDX_Control(pDX, IDC_BROWSE_BUTTON, m_cBrowseButton);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_strFileName);
	DDX_Text(pDX, IDC_STATUS_EDIT, m_strStatus);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportFlightFileDlg, CDialog)
	//{{AFX_MSG_MAP(CExportFlightFileDlg)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, OnViewButton)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseButton)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO, OnSelchangeDateCombo)
	ON_BN_CLICKED(IDC_BUTTON2, OnExcludeButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportFlightFileDlg message handlers

BOOL CExportFlightFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.pszText = _T("Classes to Export:");
	lvc.cx =  200;
	lvc.fmt =LVCFMT_LEFT;
	m_cClassList.InsertColumn(0,&lvc);

	m_cClassList.SetAvailableClasses( );
	m_pDoc->LoadDateComboBox( m_cDateCombo );

	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);


	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );

	FormatFlightLogZipName( m_strFileName, m_pDoc->m_iSSA_ID, pcTime) ;
	//m_strFileName=pcTime->Format("WinscoreFlights%b%d.zip");

	if( strPath.GetLength()>0 ) 
		m_strFileName=strPath+m_strFileName;

//	m_cViewButton.EnableWindow(false);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE 
}

void CExportFlightFileDlg::OnOK() 
{
	CWinscoreListCtrl cStatus;
	cStatus.m_fReport=false;
	CScoreRecordList cScoreRecordList;
	EClass		aeClasses[NUMCLASSES];
	int nRecords=0;
	CSummary cSummary;

	size_t bSize=256;
	TCHAR *pTemp=new TCHAR[bSize];
	_dupenv_s(&pTemp, &bSize, _T("TEMP") );
	if( pTemp==NULL ) 
		_dupenv_s(&pTemp, &bSize, _T("TMP") );
	CString strWorkDIR=pTemp;
	delete pTemp;

	BOOL fCondense=GetWinscoreInt( FTPCONDENSE, FALSE ); 
	int iTime=GetWinscoreInt( FTPCONDENSESEC, 4 ); 
	int iTopRank=100;
	if( fCondense ) iTopRank=GetWinscoreInt( FTPTRANSFERTOP, 10 ); 



	UpdateData();

	int CURRENTWFFVERSION=3;

	int			nClasses=m_cClassList.GetNumCheckedClasses();
	if( nClasses==0 ) 
		{
		AfxMessageBox(_T("One or more classes must be selected for export."));
		return;
		}

	for( int i=0; i<nClasses; i++ )
		aeClasses[i]=m_cClassList.GetCheckedClass(i);


	if( m_strFileName.GetLength()==0 ) 
		{
		AfxMessageBox(_T("Please enter a file name."));
		return;
		}

	// Check if the file exists, ask if ok to overwrite if so
	CFileStatus rStatus;
	if( CFile::GetStatus( m_strFileName, rStatus ) )
		{
		if( AfxMessageBox( m_strFileName+_T(" already exists, Overwrite?"), MB_ICONQUESTION  | MB_YESNO )==IDNO ) return;
		}

	int iSel=m_cDateCombo.GetCurSel();
	if( iSel<0 ) 
		{
		AfxMessageBox( _T("Invalid Date Selection"));
		return;
		}
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	int nNumTasks= m_pDoc->m_taskList.GetNumTasks( *pcTime, nClasses, aeClasses);
	if( nNumTasks==0 )
		{
		AfxMessageBox(_T("Warning - No tasks have been defined for this day."));
		return;
		}

	int nFlights= m_pDoc->m_FlightList.GetNumberFlights( *pcTime, nClasses, aeClasses );
	if( nFlights==0 ) 
		{
		AfxMessageBox(_T("There are no flights in the system for this day."));
		return;
		}

	CWaitCursor cWait;

	int nMaxRange=(nFlights);
	m_cProgress.SetRange(0,nMaxRange);
	m_cProgress.SetPos(0);
	int nExported=0;

	try
		{
		CZipArchive cZip;

		cZip.Open(LPCSTR(m_strFileName),CZipArchive::create);

		for( int i=0; i<nClasses; i++ )
			{

			m_pDoc->CalculateScores( *pcTime, aeClasses[i],  cStatus, cSummary);
			m_pDoc->m_ScoreRecordList.CopySort(	*pcTime, 
												aeClasses[i],
												cScoreRecordList, 
												eCumulative);



			CStringArray strArray, strArrayName;
			m_pDoc->m_FlightList.GetFileList2(aeClasses[i], *pcTime, strArray, strArrayName, m_pDoc->m_contestantList, cScoreRecordList, iTopRank);

			for( int i=0; i<strArray.GetSize(); i++)
				{
				CFileStatus cStat;
				CString strTmp=GetNonRelativePath(strArray[i]);
				if(!CFile::GetStatus( strTmp, cStat) ) 
					continue;

				CString strZipName;
				if( fCondense )
					{
					strZipName=strWorkDIR;
					strZipName+=_T("\\");
					strZipName+=strArrayName[i];
					CIGCFile::Smash(GetNonRelativePath(strArray[i]), strZipName, iTime);
					}
				else
					strZipName=GetNonRelativePath(strArray[i]);


				if ( cZip.AddNewFile(strZipName,GetRelativePath(strArray[i],false))==false)
					{
					AfxMessageBox(_T("Error writing flight log.zip file: ")+ strArray[i]);
					};

				if( fCondense )
					DeleteFile( strZipName );

				m_cProgress.SetPos( m_cProgress.GetPos()+1); 
				nExported++;
				}
			}

		//Add the current Winscore wscx file

		CXMLMgr cMgr;
		cMgr.Initlize();
		cMgr.CreateXMLFileHeader();
		cMgr.CreateRoot(_T("Winscore"));

		MSXML2::IXMLDOMNodePtr pRoot=  cMgr.GetRoot();
		m_pDoc->GetXML( cMgr, pRoot);

		CString strWSCX;
		CString strDate=pcTime->Format(_T("%d-%b-%Y"));
		strWSCX.Format(_T("%s\\Winscore_%s.wscx"), 
						strWorkDIR,
			  			strDate );
		TESTHR(cMgr.Save(strWSCX) );

		if ( cZip.AddNewFile(strWSCX)==false)
			{
			AfxMessageBox(_T("Error writing Winscore database to .zip file: ")+ strWSCX);
			};
		DeleteFile(strWSCX);

		cZip.Close();

		m_cProgress.SetPos(nMaxRange);

		CString strMess=m_strFileName;
		strMess+=_T(" Successfully created!");
		AfxMessageBox(strMess,MB_ICONINFORMATION);

		CString strPath;

		CFile  cFile;
		CFileException e;
		if( cFile.Open( m_strFileName, CFile::modeRead, &e ) )
			{
			m_strFileName=cFile.GetFilePath();
			CString strPath=m_strFileName;
			strPath.Replace(LPCSTR(cFile.GetFileName()), _T(""));
			AfxGetApp()->WriteProfileString( REGSECTION, EXPORTPATH, strPath );
			cFile.Close();
			}

		m_strStatus.Format( _T("%d flights exported to file: %s"), nExported, m_strFileName );
		}
	catch (...)
		{
		AfxMessageBox( _T("Error writing .zip file."),  MB_ICONSTOP   );
		}

	UpdateData(FALSE);

	m_cCancelButton.SetWindowText(_T("Close"));

}

void CExportFlightFileDlg::OnViewButton() 
{
	DWORD error;
	PROCESS_INFORMATION processInfo;
    STARTUPINFO si;
	GetStartupInfo(&si);

	TCHAR Command[1024];
	strncpy_s(Command, 1024, _T("WSViewer.exe "), 100) ;
	strcat_s(Command,  m_strFileName ); 

    BOOL result=CreateProcess(	NULL,//strUploadProgram, 
								Command, 
								NULL, NULL, FALSE, 
								CREATE_DEFAULT_ERROR_MODE, 
								NULL, 
								NULL, 
								&si, 
								&processInfo);	
	if( result==0 )
		{
		error=GetLastError();
		}

}

void CExportFlightFileDlg::OnBrowseButton() 
	{
	CString cSuffix=_T("wsf");

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );

	UpdateData();
	static TCHAR BASED_CODE szFilter[] = _T("Contests in Zip Archive (*.zip)|*.zip|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(FALSE, cSuffix, m_strFileName, OFN_OVERWRITEPROMPT, szFilter);

	if( strPath.GetLength()>0 )
		cFileDlg.m_ofn.lpstrInitialDir=strPath;

	if( cFileDlg.DoModal()==IDOK )
		{
		m_strFileName=cFileDlg.GetPathName();
		UpdateData(FALSE);
		}
}

void CExportFlightFileDlg::OnSelchangeDateCombo() 
{
	int iSel=m_cDateCombo.GetCurSel();
	if( iSel<0 ) return;
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	//m_strFileName=pcTime->Format(_T("WinscoreFlights%b%d.zip"));

	FormatFlightLogZipName( m_strFileName, m_pDoc->m_iSSA_ID, pcTime) ;

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );

	if( strPath.GetLength()>0 ) 
		m_strFileName=strPath+m_strFileName;

	UpdateData(FALSE);	
}

void CExportFlightFileDlg::OnExcludeButton() 
{
//	CExcludeContestantsDlg dlg;	
//	dlg.m_pDoc=m_pDoc;
//	if( dlg.DoModal()==IDOK )
//		{
//		dlg.m_pDoc->SetModifiedFlag();
//		}

	CFlightLogTransferSettings dlg;	
	dlg.m_pDoc=m_pDoc;
	if( dlg.DoModal()==IDOK )
		{
		dlg.m_pDoc->SetModifiedFlag();
		}

}

