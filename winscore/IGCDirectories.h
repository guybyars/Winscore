#pragma once

#include "WinscoreDoc.h"

class CIGCDirectories
{
public:
	CIGCDirectories(void);

	~CIGCDirectories(void);

	bool				 FindIGCFileName( CString strIGCFileName, CString& strFoundIGCFileName );
	static	bool		 ImportCAI( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFilePath,  CString &strOutputFileName, CString &strStatus, bool &bSecure );
	static	bool		 ImportIGC( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFileName, CString &strStatus );
	static	int			 FindIGCFilesInDirectory( CString strPath, CString strType, CTime cDate, int nDays, CStringArray &strArrayPath, CStringArray &strArray, int nLevels );
	static  CString      GetFolderDate(CTime cDate);
			CString		 GetFlightLogPath(CTime cDate=0, bool bPreContest=false);
			int			 FindIGCFiles( CTime cDate,  int nDays, CStringArray &strArray );
			void		 GetFlightLogFolders(CStringArray &strArray);
			void		 SetFlightLogFolders(CStringArray &strArray);
			void		 SaveFlightLogFolders();
			bool         GetAutoDateFlag() {return m_bLogPathsByDate;};
			bool		 GetAutoDateFolders(CWinscoreDoc *pDoc, CStringArray &straDateFolders);
			void		 SetAutoDate(bool);
			CString      GetImportText();

static CString GetNonRelativePath(CString &strFileIn);

private:
	int		LoadFolders();
	int     m_nLevels;
	bool    m_bLogPathsByDate;

	CStringArray m_straLogFolders;
	CStringArray m_straSearchFolders;
};

