#include "stdafx.h"
#include "global_prototypes.h"
#include "sua.h"
#include "suautil.h"
#ifndef _CAIEXPLR
#include "dontshowdlg.h"
#endif
#include <fstream>


static CSUASectionArray cSectionArray;

CSUASectionArray* GetSUAArray( )
	{
	return &cSectionArray;
	}
#ifndef _CAIEXPLR
int LoadSUA( CTurnpointArray &turnpointArray)
	{
	int nSections=0;
	int nSections2=0;
	BOOL bSUAIgnore=false;
	BOOL bSUAProcessing=false;
	CString strAirspaceFilename;
	CString strAirspaceFilename2;
	CStringArray strAirspaceArray;
	CTurnpoint*  pHomePoint=NULL;

	try
		{
		bSUAIgnore			= (BOOL)GetWinscoreInt( SUA_IGNORE, DEFAULT_SUA_IGNORE);
		strAirspaceFilename	= GetWinscoreString(	SUADISPLAYFILE, DEFAULT_SUADISPLAYFILE );
		strAirspaceFilename2= GetWinscoreString(	SUADISPLAYFILE2, "" );
		bSUAProcessing		= GetWinscoreInt(		SUADISPLAY, DEFAULT_SUADISPLAY );
		if( !bSUAProcessing ) return 0;
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in function LoadSUA reading registry."));
		return 0;
		}

	// Been here, done this!
	if( cSectionArray.GetSize()>0 && strAirspaceFilename==cSectionArray.m_strFileName )
		return cSectionArray.GetSize();

	try {
		nSections=ReadSUAFile(strAirspaceFilename, strAirspaceArray);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in ReadSUAFile"));
		return 0;
		}

	try {
		nSections2=ReadSUAFile(strAirspaceFilename2, strAirspaceArray);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in ReadSUAFile for 2ndary SUA"));
		return 0;
		}

	nSections+=nSections2;

	if( nSections==0  ) 
		{
		CDontShowDlg dlg(	_T("ErrorReadingAirspace"),
							_T("Could not read airspace file: ")+strAirspaceFilename);
		dlg.DoModalConditional();
		return 0;
		}

	pHomePoint=turnpointArray.GetHomePoint();

	double dCheckDist=turnpointArray.GetMaxDistanceFromHome()+30.;

	try {
		for( int iSection=0; iSection<strAirspaceArray.GetSize(); iSection++ )
			{
			CSUASection *pSUASection=NULL;
			try {
				pSUASection=new CSUASection(strAirspaceArray[iSection]);
				}
			catch(...)
				{
				CString cError;
				cError.Format(_T("Unhandled exception in function LoadSUA during new CSUASection operation, iSection=%d"),iSection);
				AfxMessageBox(cError);
				throw;
				}

			if( !bSUAIgnore ||
				(pHomePoint && !pSUASection->FarFrom(*pHomePoint,dCheckDist)) ) 
				cSectionArray.Add(pSUASection);
			else
				delete pSUASection;
			}

		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in function LoadSUA processing airspace data."));
		return 0;
		}

	cSectionArray.m_strFileName=strAirspaceFilename;

	return cSectionArray.GetSize();
	}

#endif

int ReadSUAFile(CString &cFileIn, CStringArray &strArray )
	{
#define MAXLINELEN		256
	// Read the important stuff from the SUA file and store it in a char string
	CString strBuild;
	int nSections=0;
	int iLinesRead=0;

	char  cBuff[MAXLINELEN];

	std::ifstream	cIfstream;

	if( cFileIn.GetLength()==0 ) return 0;

	CString cFileName=GetNonRelativePath(cFileIn);

	try {
		//  Use the ifstream class to do I/O on the ascii files.
		cIfstream.open(cFileName);
		if (cIfstream.is_open()==0)
			{
			return 0;
			}
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in ReadSUAFile ifstream::open"));
		return 0;
		}


	bool bInclude=true;
	while( cIfstream.good() )
		{
		iLinesRead++;
		try {
			cIfstream.getline( cBuff, MAXLINELEN );
			if( cIfstream.fail() ) break;
			}
		catch(...)
			{
			AfxMessageBox(_T("Unhandled exception in ReadSUAFile cIfstream.getline"));
			return 0;
			}

		CString cLine=cBuff;
		if( cLine.GetLength()==0 ) continue;

        if( cLine[0]=='#' ) continue;

		cLine.TrimRight();

		if( cLine.Find(_T("END"))==0	) break;

		if( cLine.Find(_T("INCLUDE=YES"))>=0 )
			{
			bInclude=true;
			continue;
			}
		if( cLine.Find(_T("INCLUDE=NO"))>=0 )
			{
			bInclude=false;
			continue;
			}

		if( !bInclude ) continue;	

		if( cLine.Find(_T("POINT="))>=0 )
			{
			int iPos=0;
			int iStart=5;
			iPos=cLine.Find(_T("N"), iStart);
			if( iPos<0 )
				iPos=cLine.Find(_T("S"), iStart);
			if( iPos<0 )
				{
				CString cWarn;
				cWarn.Format(_T("Found invalid line in sua file at line %d =%s."),iLinesRead,cLine);
				AfxMessageBox(cWarn);
				return 0;
				}
			}


		if( cLine.Find(_T("TITLE="))>=0	) 
			{
			nSections++;
			if( nSections>1 && strBuild.GetLength()>0 )
				strArray.Add(strBuild);
			strBuild=_T("");
			}

		strBuild+=cLine+'\n';
		}

	cIfstream.close();

	if(strBuild.GetLength()>0 )	strArray.Add(strBuild); 

	return nSections;
	}
