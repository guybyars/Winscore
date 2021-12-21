//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ImportFlightsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "winscoredoc.h"
#include "igcfile.h"
#include "ImportFlightsDlg.h"
#include "global_prototypes.h"
#include "flight.h"
#include <fstream>
#include "MainFrm.h"
#include "IGCDirectories.h"
#include <process.h>
#include "ResizingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int nImported=0;

/////////////////////////////////////////////////////////////////////////////
// CImportFlightsDlg dialog
//IMPLEMENT_DYNAMIC(CImportFlightsDlg, CResizingDialog)


CImportFlightsDlg::CImportFlightsDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CImportFlightsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportFlightsDlg)
	m_strStatus = _T("");
	m_iUploadRadio = 0;
	//}}AFX_DATA_INIT

    SetControlInfo(IDOK,				ANCHORE_BOTTOM);
    SetControlInfo(IDC_STATUS,		RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDC_STATIC_INFO,	RESIZE_HOR );

}


void CImportFlightsDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportFlightsDlg)
	DDX_Control(pDX, IDC_IMPORT_FILES, m_cImportFilesButton);
	DDX_Text(pDX, IDC_STATUS, m_strStatus);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_INFO, m_cStaticInfo);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgress);
}


BEGIN_MESSAGE_MAP(CImportFlightsDlg, CResizingDialog)
	//{{AFX_MSG_MAP(CImportFlightsDlg)
	ON_BN_CLICKED(IDC_IMPORT_FILES, OnImportFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportFlightsDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CImportFlightsDlg message handlers

/*
void CImportFlightsDlg::OnUpload() 
	{
	UploadFlight();
	}
*/


void CImportFlightsDlg::ImportIGCFile()
	{
	
	CFileStatus rSourceStatus, rDestStatus;
	CString strInputFile;
	CIGCDirectories cIGCDirs;

	/* To allow the user to select multiple files, set the OFN_ALLOWMULTISELECT 
	  flag before calling DoModal. You need to supply your own filename buffer to 
	  accommodate the returned list of multiple filenames. Do this by replacing 
	  m_ofn.lpstrFile with a pointer to a buffer you have allocated, after 
	  constructing the CFileDialog, but before calling DoModal. Additionally, 
	  you must set m_ofn.nMaxFile with the number of characters in the buffer pointed 
	  to by m_ofn.lpstrFile. */

	m_strStatus=_T("");

	static TCHAR BASED_CODE szFilter[] = _T("IGC Flight Data Files (*.igc)|*.igc|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(TRUE, _T(".igc"),	_T("*.igc"), OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT , szFilter);
	cFileDlg.m_ofn.lpstrTitle=_T("Select IGC Files");

	cFileDlg.m_ofn.nFilterIndex=AfxGetApp()->GetProfileInt(REGSECTION, IMPORTINDEX, 2);

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, IMPORTPATH  );
	cFileDlg.m_ofn.lpstrInitialDir=strPath;

	TCHAR  cBuf[10000]=_T("");
	cFileDlg.m_ofn.lpstrFile=cBuf;//(char*)malloc( 100000*sizeof(char) );
	cFileDlg.m_ofn.nMaxFile=sizeof( cBuf);

	m_cImportArray.RemoveAll();
	m_cOutputArray.RemoveAll();

	if( cFileDlg.DoModal()==IDOK )
		{

		CWaitCursor cWait;
		AfxGetApp()->WriteProfileInt(REGSECTION, IMPORTINDEX, cFileDlg.m_ofn.nFilterIndex);

		int iCount=0;
		POSITION pos=cFileDlg.GetStartPosition();
		while(pos)
			{
			cFileDlg.GetNextPathName( pos );
			iCount++;
			}
		m_cProgress.SetRange(0,iCount);
		iCount=0;

		pos=cFileDlg.GetStartPosition();
	    BOOL bFirst=true;
		while(pos)
			{
			m_cProgress.SetPos(iCount++);
			strInputFile=cFileDlg.GetNextPathName( pos );

			CFile cFile;
			cFile.Open(strInputFile, CFile::modeRead);
			CString strFileName=cFile.GetFileName();

			if( bFirst )
				{
				CString strPath=cFile.GetFilePath();
				strPath.Replace(LPCTSTR(cFile.GetFileName()), _T(""));
				AfxGetApp()->WriteProfileString( REGSECTION, IMPORTPATH, strPath );
				bFirst=false;
				}

			cFile.Close();

			if( strFileName.Find(_T(".IGC"))!=(-1) || strFileName.Find(_T(".igc"))!=(-1) )
				{
				CIGCFile cIGCFile(strInputFile);
				m_strLogPath	=cIGCDirs.GetFlightLogPath(CTime(cIGCFile.m_iYear,cIGCFile.m_iMonth,cIGCFile.m_iDay,0,0,0) );
				CString strOutputFileName=m_strLogPath+_T("\\")+strFileName;

				if(  CIGCDirectories::ImportIGC( strInputFile, strFileName, m_strLogPath, strOutputFileName, m_strStatus ) )
					{
					CString strSecureStatus;
					if( CheckIGCSecurity( strInputFile,  strSecureStatus ) )
						m_pDoc->LogPassed(strFileName);
					else
						{
						m_strStatus+=strSecureStatus;
						m_pDoc->LogFailed(strFileName);
						}
					UpdateWindow();
					UpdateData(FALSE);
					}

				}
			else
				{
				// Bogus file
				m_strStatus+=strFileName+_T(" skipped. \r\n");
				

				} // End if igc file


			UpdateData(FALSE);

			} // end while (pos)

		} // end modal ok

		UpdateData(FALSE);
	}


void CImportFlightsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


BOOL CImportFlightsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CIGCDirectories cIGCDirs;

	m_strLogPath	=cIGCDirs.GetFlightLogPath();

	CFileStatus rStatus;
	if( CFile::GetStatus( m_strLogPath, rStatus )==FALSE )
		{
		AfxMessageBox(_T("Invalid Flights path, check flight log settings."));
		}

	m_cStaticInfo.SetWindowTextA(cIGCDirs.GetImportText());

	TCHAR cBuff[512];

    GetShortPathName(m_strLogPath, cBuff, 512);
	m_strLogPath=cBuff;

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportFlightsDlg::OnImport() 
	{
	ImportIGCFile();
	}

void CImportFlightsDlg::ConvertImportedFiles(CStringArray &cInputFiles )
{
	if( cInputFiles.GetSize() ==0 ) return;

	UpdateData(FALSE);	
}

/*

void CImportFlightsDlg::OnRadioUpload() 
{
	m_cManufacturerText.EnableWindow(true);
	m_cImportFilesButton.EnableWindow(false);
	m_cSourceCombo.EnableWindow(true);
	m_cUploadButton.EnableWindow(true);
	m_iUploadRadio = 0;
	
}

void CImportFlightsDlg::OnRadioImport() 
{
	m_cManufacturerText.EnableWindow(false);
	m_cImportFilesButton.EnableWindow(true);
	m_cSourceCombo.EnableWindow(false);
	m_cUploadButton.EnableWindow(false);
	m_iUploadRadio = 1;

}
*/
void CImportFlightsDlg::OnImportFiles() 
{
	ImportIGCFile();
	
}
void CImportFlightsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	
}

void CImportFlightsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}
