
#include "stdafx.h"
#include "Global_Prototypes.h"
#include "Winscore.h"
#include "WinscoreDoc.h"
#include "IGCDirectories.h"
#include "caiapi.h"

static CString strBasePath="FlightLogs";

CIGCDirectories::CIGCDirectories(void)
{
	m_nLevels=1;
	LoadFolders();
	m_bLogPathsByDate = GetWinscoreInt( DATEDLOGPATHS, 1)==1?TRUE:FALSE;
}


CIGCDirectories::~CIGCDirectories(void)
{
}


int CIGCDirectories::LoadFolders()
	{
	char buff[10];

	CString strReg=GetWinscoreString( LOGPATH, _T("")  );
	if( strReg.GetLength() >0 ) 
		{
		m_straLogFolders.Add(strReg); 
		m_straSearchFolders.Add(CIGCDirectories::GetNonRelativePath(strReg)); 
		}

	int iNumFolders		=GetWinscoreInt( LOGPATHNUMFOLDERS, 0);

	for( int i=0; i<iNumFolders; i++ )
		{
		CString strSection=LOGPATH;
		_itoa_s( i+1, buff, 10, 10);
		strSection+=buff;
		CString strLogPath	=GetWinscoreString( strSection, _T("")  );
		if( strLogPath.GetLength()>0 )
			{
			m_straLogFolders.Add(strLogPath);
			m_straSearchFolders.Add(CIGCDirectories::GetNonRelativePath(strLogPath)); 
			}
		}

	return m_straLogFolders.GetSize();
	}


CString CIGCDirectories::GetNonRelativePath(CString &strFileIn)
	{
	char szDrive[3];
	memset(szDrive,0,3);
	_splitpath_s(strFileIn, szDrive, 3, NULL, 0, NULL, 0, NULL, 0);
	if( szDrive[1]==':' ) 
		return strFileIn; // Already full
	else
		return GetWSCPath()+"\\"+strFileIn;
	}

int CIGCDirectories::FindIGCFilesInDirectory( CString strPath, CString strType, CTime cDate,  CStringArray &strArrayPath, CStringArray &strArray, int nLevels )
	{
	CFileFind cFindFile;
	CString strSearchName, strLongSearchName;
	int nFound=0;

	strSearchName.Format(_T("*.%s"),  strType  );  //Find all .log files 


	CString strShortPrefix=CIGCFile::GetDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() );
	CString strLongPrefix=CIGCFile::GetLongDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay()) ;

	CString strTemp=strPath+_T("\\");
	strTemp+=strSearchName;
	BOOL fMore=true;

	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();
			if( cDate==0 ) 
				{
				strArrayPath.Add(cFindFile.GetFilePath());
				strArray.Add(cFindFile.GetFileName());
				}
			else
				{
				CString strLowerFileName=cFindFile.GetFileName();
				strLowerFileName.MakeLower();
				if(  strLowerFileName.Find(strShortPrefix)==0 || strLowerFileName.Find(strLongPrefix)==0			)
					{
					// Dates match
					strArrayPath.Add(cFindFile.GetFilePath());
					strArray.Add(cFindFile.GetFileName());
					}
				else if( strLowerFileName.GetLength()!=12 && strLowerFileName.GetLength()!=25 )
					{
					// Non compliant IGC file name, crack the file and check the date
					try
						{
						CIGCFile cIGCFile(cFindFile.GetFilePath());
						if( cIGCFile.m_iYear	==cDate.GetYear() &&
							cIGCFile.m_iMonth	==cDate.GetMonth() &&
							cIGCFile.m_iDay		==cDate.GetDay()	)
							{
							strArrayPath.Add(cFindFile.GetFilePath());
							strArray.Add(cFindFile.GetFileName());
							}
						}
					catch(...)
						{
						continue;
						}
					}

				}
			nFound++;
			}
		}
	

	if( nLevels<=0 ) return nFound;

	// Find in the next level.
	strTemp=strPath+_T("\\*");
	fMore=true;
	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();

			if( cFindFile.IsDots()	) continue;

			if(cFindFile.IsDirectory() )
				{
				nFound+=CIGCDirectories::FindIGCFilesInDirectory( cFindFile.GetFilePath(), strType, cDate, strArrayPath, strArray, nLevels-1 );
				}
			}
		}

	return nFound;
	}



/*

int CIGCDirectories::FindIGCFilesInDirectory( CString strPath, CString strType, CTime cDate,  CStringArray &strArrayPath, CStringArray &strArray, int nLevels )
	{
	CFileFind cFindFile;
	CString strSearchName, strLongSearchName;
	int nFound=0;

	if( cDate==0 )
		strSearchName.Format(_T("*.%s"),  strType  );  //Find all .log files regardless of date.
	else
		{
		strSearchName.Format(    _T("%s*.%s"), CIGCFile::GetDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() ),		strType  );
		strLongSearchName.Format(_T("%s*.%s"), CIGCFile::GetLongDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() ),	strType  );
		}

	CString strTemp=strPath+_T("\\");
	strTemp+=strSearchName;
	BOOL fMore=true;

	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();
			strArrayPath.Add(cFindFile.GetFilePath());
			strArray.Add(cFindFile.GetFileName());
			nFound++;
			}
		}
	
	if( cDate>0 )
		{
		// Find the long file named igc files.
		strTemp=strPath+_T("\\");
		strTemp+=strLongSearchName;
		fMore=true;

		if( cFindFile.FindFile(strTemp) )
			{
			while( 1 )
				{
				if( !fMore ) break;
				fMore=cFindFile.FindNextFile();
				strArrayPath.Add(cFindFile.GetFilePath());
				strArray.Add(cFindFile.GetFileName());
				nFound++;
				}
			}
		}

	if( nLevels<=0 ) return nFound;

	// Find in the next level.
	strTemp=strPath+_T("\\*");
	fMore=true;
	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();

			if( cFindFile.IsDots()	) continue;

			if(cFindFile.IsDirectory() )
				{
				nFound+=CIGCDirectories::FindIGCFilesInDirectory( cFindFile.GetFilePath(), strType, cDate, strArrayPath, strArray, nLevels-1 );
				}
			}
		}

	return nFound;
	}

*/


int CIGCDirectories::FindIGCFiles( CTime cDate,  CStringArray &strArray )
	{
	CFileStatus rStatus;
	CFileFind cFindFile;
	CStringArray strDum;

	if( m_bLogPathsByDate )
		{
		CIGCDirectories::FindIGCFilesInDirectory( GetNonRelativePath(strBasePath), "igc", cDate,  strArray, strDum, 1 );
		}
	else
		{
	for( int iPath=0; iPath<m_straSearchFolders.GetSize(); iPath++)
		{
		CIGCDirectories::FindIGCFilesInDirectory( m_straSearchFolders[iPath], "igc", cDate,  strArray, strDum, m_nLevels );
		}
		}

	//Now remove duplicates
	for( int i=0; i<strArray.GetSize(); i++ )
		{
		char szFileName1[80], szFileName2[80];
		memset(szFileName1,0,80);
		_splitpath_s(strArray[i], NULL, 0, NULL, 0, szFileName1, 80, NULL, 0);

		for( int j=i+1; j<strArray.GetSize(); j++ )
			{
			memset(szFileName2,0,80);
			_splitpath_s(strArray[j], NULL, 0, NULL, 0, szFileName2, 80, NULL, 0);

			CString str1(szFileName1), str2(szFileName2);
			if( str1.CompareNoCase(str2)==0 )
				{
				strArray.RemoveAt(j);
				}
			}
		}

	return strArray.GetSize();
	}


bool CIGCDirectories::FindIGCFileName( CString strIGCFileName, CString& strFoundIGCFileName )
	{
	CStringArray strArray, strArrayPath;

	int nFound=FindIGCFiles( 0,  strArrayPath );
	if( nFound==0 ) return false;

	char szFileName1[80], szFileName2[80];
	memset(szFileName1,0,80);
	_splitpath_s(strIGCFileName, NULL, 0, NULL, 0, szFileName1, 80, NULL, 0);

	for( int iFile=0; iFile<strArrayPath.GetSize(); iFile++)
		{
		memset(szFileName2,0,80);
		_splitpath_s(strArrayPath[iFile], NULL, 0, NULL, 0, szFileName2, 80, NULL, 0);	

		CString str1(szFileName1), str2(szFileName2);
		if( str1.CompareNoCase(str2)==0 )
			{
			strFoundIGCFileName=strArrayPath[iFile];
			return true;
			}
		}
	return false;
	}

void CIGCDirectories::GetFlightLogFolders(CStringArray &strArray)
		{
		for( int i=0; i<m_straLogFolders.GetSize(); i++ ) strArray.Add(m_straLogFolders[i]);
		}

void CIGCDirectories::SetFlightLogFolders(CStringArray &strArray)
		{
		m_straLogFolders.RemoveAll();
		m_straSearchFolders.RemoveAll();
		for( int i=0; i<strArray.GetSize(); i++ ) 
			{
			m_straLogFolders.Add(strArray[i]);
			m_straSearchFolders.Add(CIGCDirectories::GetNonRelativePath(strArray[i]));
			}
		}

void	CIGCDirectories::SaveFlightLogFolders()
		{
		char buff[10];
		if( m_straLogFolders.GetSize()==0 ) 
			{
			//Nothing here, remove all
			RemoveWinscoreEntry(LOGPATH);
			RemoveWinscoreEntry(LOGPATHNUMFOLDERS);
			int i=1;
			while(i<100)
				{
				CString strSection=LOGPATH;
				_itoa_s(i, buff, 10, 10);
				strSection+=buff;
				if( !RemoveWinscoreEntry( strSection ) ) break;
				i++;
				}
			return;
			}

		WriteWinscoreInt( LOGPATHNUMFOLDERS, m_straLogFolders.GetSize()-1);

		for( int i=0; i<m_straLogFolders.GetSize(); i++ )
			{
			CString strSection=LOGPATH;
			if( i>0 ) 
				{
				_itoa_s(i, buff, 10, 10);
				strSection+=buff;
				}

			WriteWinscoreString( strSection, m_straLogFolders[i]  );
			}
		}

CString CIGCDirectories::GetFlightLogPath(CTime cDate)
	{
	CString strLogPath;
	if( m_bLogPathsByDate ) 
		{
		strLogPath=strBasePath;
		if( cDate>0 )
			{
			strLogPath+="\\";
			strLogPath+=GetFolderDate(cDate);
			}
		strLogPath=GetNonRelativePath(strLogPath);
		CreateDirectory(GetNonRelativePath(strBasePath), NULL);
		CreateDirectory(strLogPath, NULL);
		return strLogPath;
		}
	else
	{
	CString strReg=GetWinscoreString( LOGPATH, _T("")  );
	if( strReg.GetLength() >0 ) 
		{
		return GetNonRelativePath(strReg);
		}

	CString strMyDocuments=FindMyDocumentsPath();
		strLogPath=strMyDocuments+"\\Winscore\\FlightLogs";
	CreateDirectory(strLogPath, NULL);
	return strLogPath;
		}


	}

CString CIGCDirectories::GetFolderDate(CTime cDate)
	{
	CString strFolder;
	strFolder=cDate.Format("%b%d");
	return strFolder;
	}

bool CIGCDirectories::GetAutoDateFolders(CWinscoreDoc *pDoc, CStringArray &straDateFolders)
	{
	if( pDoc==NULL ) return false;
	CString strFolderBase;
	straDateFolders.Add(strBasePath);

	strFolderBase=strBasePath+"\\";
	straDateFolders.Add(strFolderBase+GetFolderDate(pDoc->m_cPracticeDay1));
	straDateFolders.Add(strFolderBase+GetFolderDate(pDoc->m_cPracticeDay2));
	for( int i=0; i<pDoc->m_iNumContestDays; i++ )
		{
		straDateFolders.Add(strFolderBase+GetFolderDate(pDoc->m_caContestDays[i]));
		}
	return true;
	}

void CIGCDirectories::SetAutoDate(bool bDate)
	{
	m_bLogPathsByDate=bDate;
	if( bDate==true )
		WriteWinscoreInt( DATEDLOGPATHS, 1);
	else
		WriteWinscoreInt( DATEDLOGPATHS, 0);
	}



bool CIGCDirectories::ImportCAI( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFilePath, CString &strOutputFileName, CString &strStatus, bool &bSecure )
	{
	bSecure=false;
	CFileStatus rSourceStatus, rDestStatus;

	strOutputFileName=strInputFileName;
	strOutputFileName.Replace(_T(".cai"),_T(".igc"));
	strOutputFileName.Replace(_T(".CAI"),_T(".igc"));
	CString strUploadFileName=strLogPath+_T("\\")+strOutputFileName;

	if( CFile::GetStatus( strInputFilePath, rSourceStatus )		&& 
		CFile::GetStatus( strUploadFileName, rDestStatus )		)
		{						
		strStatus+=strInputFilePath+_T(" has already been imported\r\n");
		return false;
		}

	BOOL bSecure1=FALSE;
	char	strCreatedFileName[1024];

	int iRet= CAIConvertFile(  strInputFilePath,
								strLogPath,
								strCreatedFileName,
								&bSecure1
								);
	if( iRet==CER_OK )
		{
		strOutputFilePath=strLogPath+_T("\\")+strCreatedFileName;
		// Copy the .cai file over too.

		 CString strCopyTo=strLogPath+_T("\\")+strInputFileName;
		 CopyFile(strInputFilePath, strCopyTo, FALSE );

		}
	else
		{
		strStatus+=_T("ERROR - Could not convert ")+strInputFilePath+_T(" to IGC format\r\n");
		return false;
		}

	if( !bSecure1 )
		{
		strStatus+=_T("SECURITY WARNING - Security check failed for ")+strInputFilePath+_T("\r\n");
		}
	else
		bSecure=true;


	CIGCFile igcFile(strOutputFilePath);
	if( igcFile.m_bValid )
		strStatus+=_T("CAI file ")+CString(strCreatedFileName)+_T(" for ")+igcFile.m_strGliderID+_T(", ")+igcFile.m_strCompetitionID+" - "+igcFile.m_strPilot+_T(" sucessfully imported.\r\n");
	else
		strStatus+=_T("ERROR Reading - ")+strOutputFileName+_T("\r\n");
	
	return true;
	}




bool CIGCDirectories::ImportIGC( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFileName, CString &strStatus )
	{

	CString strLongName;
	if( CIGCFile::GetARecord(strInputFilePath, strLongName) )
		{
		strOutputFileName=strLogPath;
		strOutputFileName+=_T("\\");
		strOutputFileName+=strLongName;
		}
	else
		{
		strStatus+=strInputFileName+_T(" does not contain a valid date record.\r\n");
		return false;
		}

	CFileStatus rStatus;
	if( CFile::GetStatus(strOutputFileName, rStatus) )
		{
		strStatus+=strInputFilePath+_T(" has already been copied to ");
		strStatus+=strOutputFileName+_T("\r\n");
		return false;
		}

	if( CopyFile(strInputFilePath, strOutputFileName, FALSE )==0 )
		{
		// copy failed
		TCHAR cBuff[512];
		GetShortPathName(strInputFilePath, cBuff, 512);
		if( strOutputFileName.CompareNoCase(cBuff)==0 )
			{
			strStatus+=strInputFilePath+_T(" has already been copied to ");
			strStatus+=strOutputFileName+_T("\r\n");
			return false;
			}
		else
			{
			CString cError;
			cError.Format(_T("System error %d while attempting to copy IGC file."),GetLastError());
			AfxMessageBox(cError);
			return false;
			}
		}

	CIGCFile igcFile(strOutputFileName);
	if( igcFile.m_bValid )
		strStatus+=_T("IGC file ")+strInputFileName+_T(" for ")+igcFile.m_strGliderID+", "+igcFile.m_strCompetitionID+" - "+igcFile.m_strPilot+_T(" sucessfully copied to: ")+strOutputFileName+_T("\r\n");
	else
		{
		strStatus+=_T("ERROR Reading - ")+strOutputFileName+_T("\r\n");
		return false;
		}

	return true;
	}


CString   CIGCDirectories::GetImportText()
	{
	if( m_bLogPathsByDate )
		{
		CString cText="Import will copy selected file(s) to the dated folder in : \n";
		cText+=GetFlightLogPath(0);;
		return cText;
		}
	else
		{
		CString cText="Import will copy selected file(s) to : \n";
		cText+=GetFlightLogPath(0);
		return cText;
		}

		
		


	}