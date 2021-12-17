//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ImportFlightsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "winscoredoc.h"
#include "caiapi.h"
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

	bool m_bCambridge=false;
	m_strStatus=_T("");
//	DWORD err;

	static TCHAR BASED_CODE szFilter[] = _T("Cambridge Logger Files (*.cai)|*.cai|IGC Flight Data Files (*.igc)|*.igc|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(TRUE, _T(".igc,.cai"),	_T("*.igc,*.cai"), OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT , szFilter);
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

			CFile cFile=CFile();
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

			if( strFileName.Find(_T(".cai"))!=(-1) || strFileName.Find(_T(".CAI"))!=(-1) )
				{
				bool bSecure;
				m_bCambridge=true;
				CString strIGCName;

				m_strLogPath	=cIGCDirs.GetFlightLogPath();
				CString strOutputFileName=m_strLogPath+_T("\\")+strFileName;

				 CIGCDirectories::ImportCAI( strInputFile, strFileName, m_strLogPath, strOutputFileName, strIGCName, m_strStatus, bSecure );
				if( bSecure )
					m_pDoc->LogPassed(strIGCName);
				else
					m_pDoc->LogFailed(strFileName);
				}
			else if( strFileName.Find(_T(".IGC"))!=(-1) || strFileName.Find(_T(".igc"))!=(-1) )
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
				

				} // End if cai or igc file


			UpdateData(FALSE);

			} // end while (pos)

		} // end modal ok

		UpdateData(FALSE);
	}

/*
void CImportFlightsDlg::UploadFlight()
	{
	PurgeUploadDir();

	CString strUploadProgram=GetUploadProgram();
	if( strUploadProgram.GetLength()==0 ) return;
	CString strUploadCommand=GetUploadCommand();


	PROCESS_INFORMATION processInfo;
    STARTUPINFO si;
	GetStartupInfo(&si);

	char Command[1024];
	strncpy(Command, LPCTSTR (m_strUploadPath), 100) ;
	strcat(Command, "\\upload.bat");

	CreateBatchFile( Command, strUploadCommand );

    BOOL result=CreateProcess(	NULL,//strUploadProgram, 
								Command, 
								NULL, NULL, FALSE, 
								CREATE_DEFAULT_ERROR_MODE, 
								NULL, 
								m_strUploadPath, 
								&si, 
								&processInfo);
	if( result )
		{
		m_eStatus=eUploading;
		m_hProcess=processInfo.hProcess;
		SetTimer( 1, 1000, NULL );
		m_strStatus="Uploading";
		m_cUploadButton.EnableWindow(FALSE);
		}	
	else
		{
		m_strStatus="System problem, could not start upload process\r\nUse the recorder's manufacturers upload software and import an IGC file into Winscore.";
		}
	UpdateData(FALSE);

}
*/
/*
void CImportFlightsDlg::ValidateUploadedFile()
{
	ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());

	CString strVerifyProgram=GetVerifyProgram();
	if( strVerifyProgram.GetLength()==0 ) return;
	CString strVerifyCommand=GetVerifyCommand();

	PROCESS_INFORMATION processInfo;
    STARTUPINFO si;
	GetStartupInfo(&si);
		
	si.dwFlags|=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_HIDE;

	char Command[1024];
	strncpy(Command, LPCTSTR (m_strUploadPath), 100) ;
	strcat(Command, "\\verify.bat");

	CreateBatchFile( Command, strVerifyCommand );

    BOOL result=CreateProcess(	NULL, //strVerifyProgram, 
								Command, 
								NULL, NULL, FALSE, 
	//							CREATE_DEFAULT_ERROR_MODE, 
								CREATE_SEPARATE_WOW_VDM,
								NULL, 
								m_strUploadPath,  //Current directory
								&si, 
								&processInfo);
	if( result )
		{
		m_eStatus=eVerifying;
		m_hProcess=processInfo.hProcess;
		SetTimer( 1, 1000, NULL );
		m_strStatus+="\r\nVerifying";
		}	
	else
		{
		m_strStatus+="\r\nSystem problem, could not start verification process\r\nUse the recorder's manufacturer's upload/verify software and import an IGC file into Winscore.";
		}
	UpdateData(FALSE);	
}
*/

void CImportFlightsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

/*
void CImportFlightsDlg::OnTimer(UINT nIDEvent) 
{
	if( m_eStatus!=eIdle )
		{
		ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());

		DWORD ExitCode;
		BOOL bStat=GetExitCodeProcess(  m_hProcess,  &ExitCode );
		if( bStat==0 || ExitCode!=STILL_ACTIVE ) 
			{
			DWORD iCode=GetLastError();
			if( m_eStatus==eUploading )
				{
				m_eStatus=eIdle;
				KillTimer(1);
				m_hProcess=0;
				m_cUploadButton.EnableWindow(TRUE);
				if( UploadedFileName().GetLength()==0 )
					{
					// Problem, no uploaded file was found
					ReportUploadStatus(1);
					}
				else
					{
					// Upload succeeded
					ReportUploadStatus(0);
					ConvertUploadedFile();
					}
				}
			else if( m_eStatus==eConverting )
				{
				bool fConverted=ValidateIGCFile();
				ReportConversionStatus((fConverted)?(1):(0));
				m_cUploadButton.EnableWindow(TRUE);
				m_eStatus=eIdle;
				KillTimer(1);
				m_hProcess=0;
				if( fConverted ) 
					{
					if( eSource==eVolkslogger )
						{
						m_strStatus+="Winscore does not attempt to perform a security check on Volkslogger flight logs,\r\nIf a security check is desired, use the IGC approved DOS program for this.";
						UpdateData(false);
						}
					else
						ValidateUploadedFile();
					}
				}
			else if( m_eStatus==eVerifying )
				{
				ReportValidationStatus(ExitCode);
				m_eStatus=eIdle;
				KillTimer(1);
				m_hProcess=0;
				}
			else if( m_eStatus==eBatchConversion )
				{
				for( int i=0; i<m_cOutputArray.GetSize(); i++)
					{
					CString strFile=m_cOutputArray[i];
					if(strFile=="") continue;
					CFileStatus rStatus;
					if( CFile::GetStatus( strFile, rStatus )==TRUE )
						{
						m_strStatus+=m_cImportArray[i]+" imported\r\n";
						m_cOutputArray.SetAt(i," ");
						}
					}

				ReportValidationStatus(ExitCode);
				m_eStatus=eIdle;
				KillTimer(1);
				m_hProcess=0;

				m_strStatus+="\r\n";

				CString cReport;
				cReport.Format("%d Cambridge file(s) imported",m_cImportArray.GetSize());
				m_strStatus+=cReport+"\r\n";
				UpdateData(false);
				}
			}
		else
			if( m_eStatus==eBatchConversion )
				{
				for( int i=0; i<m_cOutputArray.GetSize(); i++)
					{
					CString strFile=m_cOutputArray[i];
					if(strFile=="") continue;
					CFileStatus rStatus;
					if( CFile::GetStatus( strFile, rStatus )==TRUE )
						{
						m_strStatus+=m_cImportArray[i]+" imported\r\n";
						m_cOutputArray.SetAt(i,"");
						}
					}
//				m_strStatus+=">";
				UpdateData(false);
				}

		}
	
	CDialog::OnTimer(nIDEvent);
}

*/

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

/*

CString CImportFlightsDlg::GetUploadProgram()
	{
	CString str=m_strBinPath;
	str+="\\DATA-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE";

	CFileFind	cFinder;
	if( !cFinder.FindFile(str) )
		{
		CString strError="Could not find upload program: ";
		strError+=str+"\n";
		strError+="Verify Winscore is properly installed.";
		AfxMessageBox(strError);
		return "";
		}

	return str;
	}

CString CImportFlightsDlg::GetUploadCommand()
	{
	CString str=m_strBinPath+"\\DATA-";
//	CString str="DATA-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE ";
	ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());
	char strComPort[2];
	_itoa( m_iComPort, strComPort, 10 );

	if( eSource==eCambridge )
		{
		str+=" -p";
		str+=strComPort;
		str+=" -d ";
		str+=m_strUploadPath;
		str+="\"";
		}
	else if( eSource==eFilser )
		{
		str+=" ";
		str+=strComPort;
		str+=" -l -n";
		str+="\"";
		}
	else if( eSource==eVolkslogger )
		{
		}
	return str;
	}

CString CImportFlightsDlg::GetVerifyProgram()
	{
	CString str=m_strBinPath;
	str+="\\VALI-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE";

	CFileFind	cFinder;
	if( !cFinder.FindFile(str) )
		{
		CString strError="Could not find verify program: ";
		strError+=str+"\n";
		strError+="Verify Winscore is properly installed.";
		AfxMessageBox(strError);
		return "";
		}

	return str;
	}

CString CImportFlightsDlg::GetVerifyCommand()
	{
	CString str=m_strBinPath+"\\VALI-";
//	CString str="VALI-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE ";
	ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());

	if( eSource==eCambridge )
		{
		str+=UploadedFileName();
		str+="\" > ";
		str+=VerificationLogFileName();
		}
	else if( eSource==eFilser )
		{
		str+=UploadedFileName();
		str+=" ";
		str+=VerificationLogFileName();
		}
	else if( eSource==eVolkslogger )
		{
		str+=UploadedFileName();
		str+="\" > ";
		str+=VerificationLogFileName();
		}
	return str;
	}


CString CImportFlightsDlg::GetConversionProgram()
	{

	ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());
	if( eSource==eVolkslogger ) return "";

	CString str;

	str=m_strBinPath;
	str+="\\CONV-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE";

	CFileFind	cFinder;
	if( !cFinder.FindFile(str) )
		{
		CString strError="Could not find conversion program: ";
		strError+=str+"\n";
		strError+="Verify Winscore is properly installed.";
		AfxMessageBox(strError);
		return "";
		}
	return str;
	}

CString CImportFlightsDlg::GetConversionCommand()
	{
	CString str=m_strBinPath+"\\CONV-";
//	CString str="CONV-";
	str+=_gszEXEName[m_cSourceCombo.GetCurSel()];
	str+=".EXE ";
//	str+="\"";
	str+=m_strUploadPath+"\\"+UploadedFileName();
	
	ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());

	if( eSource==eCambridge )
		{
		str+=" \""+m_strUploadPath+"\"";
		}
	else if( eSource==eFilser )
		{
		}
	else if( eSource==eVolkslogger )
		{
		str="copy "+m_strUploadPath+"\\"+UploadedFileName()+" "+m_strLogPath;
		}
	return str;
	}


CString CImportFlightsDlg::UploadLogFileName()
{
return "upload\\upload.log";
}

CString CImportFlightsDlg::ConversionLogFileName()
{
return "upload\\convert.log";
}


CString CImportFlightsDlg::UploadedFileName()
{
	CString str=m_strUploadPath+"\\*.";

	CFileFind	cFinder;
	if( cFinder.FindFile(str+_gszFileType[m_cSourceCombo.GetCurSel()]) )
		{
		cFinder.FindNextFile();
		return cFinder.GetFileName();
		}
	else
		{
		return "";
		}
}



void CImportFlightsDlg::ReportUploadStatus(int ExitCode)
{
	if( ExitCode==0 )
		m_strStatus="Log was sucessfully imported!!!";
	else
		m_strStatus="ERROR importing log file.\r\nMake sure the settings are correct and the recorder is ready.\r\n";

/*	CString strFileName=UploadLogFileName();

	ifstream ifstr=ifstream( LPCTSTR (strFileName) );
	if( ifstr.is_open() )
		{
		while(!ifstr.eof() )
			{
			char strLine[80];
			ifstr.getline(strLine, 80 );
			m_strStatus+=strLine;
			m_strStatus+="\r\n";
			}
		}
	ifstr.close();
	UpdateData(FALSE);

}

void CImportFlightsDlg::ReportValidationStatus(int ExitCode)
{

	CString strFileName=VerificationLogFileName();
	m_strStatus+="\r\n";
	ifstream ifstr=ifstream( LPCTSTR (strFileName) );
	if( ifstr.is_open() )
		{
		while(!ifstr.eof() )
			{
			char strLine[1024];
			ifstr.getline(strLine, 1024 );
			m_strStatus+=strLine;
			m_strStatus+="\r\n";
			}
		}
	ifstr.close();

	UpdateData(FALSE);
}

CString CImportFlightsDlg::VerificationLogFileName()
	{
	CString str=m_strUploadPath;
	str+="\\validate.log";
	return str;
	}

void CImportFlightsDlg::ConvertUploadedFile()
{

	CString strConversionProgram=GetConversionProgram();
	CString strConversionCommand=GetConversionCommand();
	
	PROCESS_INFORMATION processInfo;
    STARTUPINFO si;
	GetStartupInfo(&si);
		
	si.dwFlags|=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_HIDE;

	char Command[1024];
	strncpy(Command, LPCTSTR (m_strUploadPath), 100) ;
	strcat(Command, "\\convert.bat");

	CreateBatchFile( Command, strConversionCommand );

    BOOL result=CreateProcess(	NULL,//strConversionProgram, 
								Command, 
								NULL, NULL, FALSE, 
								CREATE_DEFAULT_ERROR_MODE, 
								NULL, 
								NULL, 
								&si, 
								&processInfo);
	if( result )
		{
		ESource eSource=(ESource)(m_cSourceCombo.GetCurSel());
		m_eStatus=eConverting;
		m_hProcess=processInfo.hProcess;
		SetTimer( 1, 1000, NULL );
		if( eSource!=eVolkslogger ) 
			m_strStatus+="\r\nConverting uploaded file to IGC Format";
		}	
	else
		{
		m_strStatus="System problem, could not start conversion to IGC process\r\nUse the recorder's manufacturer's upload/conversion software and import an IGC file into Winscore.";
		}
	UpdateData(FALSE);	
}


void CImportFlightsDlg::ReportConversionStatus(int fConverted)
{

	if( fConverted==0 )
		{
		m_strStatus+="\r\nERROR: Upload process failed to produce a valid IGC file.\r\nTry using the manufacturers upload/conversion software and import the IGC file into Winscore";
		UpdateData(FALSE);
		return;
		}

	CString strFileName=ConversionLogFileName();
	ifstream ifstr=ifstream( LPCTSTR (strFileName) );
	if( ifstr.is_open() )
		{
		while(!ifstr.eof() )
			{
			char strLine[1024];
			ifstr.getline(strLine, 1024 );
			m_strStatus+="\r\n";
			m_strStatus+=strLine;
			}
		}
	ifstr.close();

	UpdateData(FALSE);
}



void CImportFlightsDlg::PurgeUploadDir()
{
	CFileFind	cFinder;
	CString str=m_strUploadPath+"\\*.";

//Remove Log Files
	if( cFinder.FindFile(str+"log") )
		{
		while( 1 )
			{
			BOOL fMore=cFinder.FindNextFile();
			DeleteFile( cFinder.GetFilePath());
			if( !fMore ) break;
			}
		}

	// Delete any other fdr files, but ONLY those files
	for (int i=0; i<NUM_FDRS; i++ )
		{
		if( cFinder.FindFile(str+_gszFileType[i]) )
			{
			while( 1 )
				{
				BOOL fMore=cFinder.FindNextFile();
				DeleteFile( cFinder.GetFilePath());
				if( !fMore ) break;
				}
			}
		}
}

bool CImportFlightsDlg::ValidateIGCFile()
{
	CFileFind	cFinder;
	CString strUploadedFileName=UploadedFileName();
	CString strUploadedFileFullName;
	CString strFileName;

	int iPeriod=strUploadedFileName.ReverseFind( '.' ); 
	if( iPeriod<0 ) return false;

	strFileName=m_strLogPath+"\\"+strUploadedFileName.Left(iPeriod)+".IGC";
	strUploadedFileFullName=m_strUploadPath+"\\"+strUploadedFileName.Left(iPeriod)+".IGC";
	DeleteFile(strFileName);
	if( MoveFile( strUploadedFileFullName, strFileName	)==0 ) return false;

	if( cFinder.FindFile(strFileName) )
		{
		m_strIGCFileName=strFileName;
		}
	else
		{
		return false;
		}
	return true;
}
*/
void CImportFlightsDlg::OnImport() 
	{
	ImportIGCFile();
	}

/*
void CImportFlightsDlg::CreateBatchFile(CString cBatch, CString &cCommand)
{
	ofstream ofs(cBatch, ios::out | ios::trunc);
	ofs << cCommand << "\n";
	ofs << "cls";
	ofs.close();
}

*/
void CImportFlightsDlg::ConvertImportedFiles(CStringArray &cInputFiles )
{
	if( cInputFiles.GetSize() ==0 ) return;
/*
	CString strConversionCommand="erase "+VerificationLogFileName()+"\n";

	for( int i=0; i<cInputFiles.GetSize(); i++ )
		{
		strConversionCommand+=m_strBinPath+"\\VALI-CAM.EXE "+cInputFiles[i]+" >> "+VerificationLogFileName()+"\n";
		strConversionCommand+=m_strBinPath+"\\CONV-CAM.EXE \""+cInputFiles[i]+"\" "+m_strLogPath+"\n";
//		strConversionCommand+="erase "+cInputFiles[i]+"\n";
//		strConversionCommand+="pause\n";
		}

	PROCESS_INFORMATION processInfo;
    STARTUPINFO si;
	GetStartupInfo(&si);
		
	si.dwFlags|=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_HIDE;

	char Command[1024];
	strncpy(Command, LPCTSTR (m_strUploadPath), 200) ;
	strcat(Command, "\\convert.bat");
	CreateBatchFile( Command, strConversionCommand );

    BOOL result=CreateProcess(	NULL,//strConversionProgram, 
								Command, 
								NULL, NULL, FALSE, 
								CREATE_DEFAULT_ERROR_MODE, 
								NULL, 
								LPCTSTR(m_strUploadPath), 
								&si, 
								&processInfo);
	if( result )
		{
		m_eStatus=eBatchConversion;
		m_hProcess=processInfo.hProcess;
		SetTimer( 1, 100, NULL );
		m_strStatus+="\r\nConverting Cambridge .CAI files to IGC Format";
		m_strStatus+="\r\n";
		}	
	else
		{
		m_strStatus="System problem, could not start conversion to IGC process\r\nUse the recorder's manufacturer's upload/conversion software and import an IGC file into Winscore.";
		}

  */
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
