//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(GLOBAL_PROTOTYPES_INCLUDED_)
#define	GLOBAL_PROTOTYPES_INCLUDED_

#include "Winscore.h"
#include "task.h"
#include "turnpointarray.h"
#include <atlbase.h>

#include "winscoredoc.h"
#include "suautil.h"
#include "sua.h"

#include "flight.h"
#include "wsbaro.h"
#include "GliderInfoList.h"

#include "CkHttp.h"
#include "CkHttpRequest.h"
#include "CkHttpResponse.h"


CString UnitsText(EUnits eUnits);
CString UnitsTextShort(EUnits eUnits);
CString UnitsSpeedText(EUnits eUnits);
CString UnitsSpeedTextShort(EUnits eUnits);
CString AltitudeUnitsText(EAltUnits eUnits );
double GetLatCorrection(double dLatitude);
void FindIGCFilesInDirectory( CTime cDate, CStringArray &strArray );
bool FindIGCFilesInDirectory2( CString strPath, CString strType, CTime cDate,  CStringArray &strFilePath, CStringArray &strFileName, int nLevels );

bool ImportIGC( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFileName, CString &strStatus );
bool ValidateIGC		( const TCHAR* strDLLName, const TCHAR* strIGCFile, CString &strReason );
bool ValidateIGCThread	( const TCHAR* strDLLName, const TCHAR* strIGCFile, CString &strReason );
bool CheckIGCSecurity( CString strIGCFile,  CString &strStatus );
bool UpdateHandicapListFromSSA(CGliderInfoList &cGIList);


CString  GetTempPath();
bool GetIGCPathName( CStringArray &strArray, CString &strIGCFilename, CString &strIGCPathName );
bool FindIGCFileInDirectory( CString strPath, CString strIGCFileName, CString& strFoundIGCFileName );
bool WriteColumns(CString strName, CListCtrl &cList );
bool ReadColumns(CString strName, CListCtrl &cList );

bool GetFlightSearchFolders( CStringArray& strFolders, bool bResolveRelativePaths=true );
void WriteFlightSearchFolders( CStringArray& strFolders );

CString GetWSCPath();
void    SetWSCPath(CString &strPath);
CString GetRelativePath(CString &strFull,bool bNonRelativeFull=true);
CString GetNonRelativePath(CString &strFileIn);

int LoadContestsFromSSA( CComboBox	*pCombo, CListCtrl *pList ); 

bool  uploadfile(CString strFile, CString &strUser, CString &strPass, bool bSilent, CString &strError);


BOOL	WriteWinscoreInt(LPCTSTR lpszEntry,		int nValue  );
BOOL	WriteWinscoreString(LPCTSTR lpszEntry,	LPCTSTR lpszValue  );
BOOL    RemoveWinscoreEntry(LPCTSTR lpszEntry);
int		GetWinscoreInt( LPCTSTR lpKeyName,		int nDefault );
CString GetWinscoreString( LPCTSTR lpKeyName,	LPCTSTR lpDefault);
BOOL	WriteWinscoreStringEncrypt(LPCTSTR lpszEntry,	LPCTSTR lpszValue  );
CString GetWinscoreStringDecrypt( LPCTSTR lpKeyName,	LPCTSTR lpDefault);
void GetRegistryXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode );
void ImportRegistryXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode );
void PurgeWinscoreRegistry();
void  WriteWinscoreDouble( LPCTSTR lpszEntry, double dVal );
double  GetWinscoreDouble( LPCTSTR lpszEntry, double dDefault );

//BOOL ValidateUser(CString &strName, int iRegNo, int iYear=2000 );

//int GetUserRegNo(CString &strName, int iYear=2000 );

CTime   GetDefaultDay();

void    SetDefaultDay(CTime cDate);
void    SetDefaultDayToday();

void	SerializeTime(CArchive & ar, CTime &cTime);

double  Roundoff( double dPoints);

void	Shuffle20Percent(CStringArray& cTempArray);

void GetSpeed(double dDist, CTimeSpan cTOC, double* pdSpeed);

bool DatesEqual( CTime &time1, CTime &time2 );

void DrawStandardTrailer(CDC *pDC);

int GetPageWidth(CDC *pDC);

void RemindAboutScores();
void ArmReminder();

CString FindExePath(CString strFileName);
CString FindMyDocumentsPath();
CString GetHandicapFileName(bool bCheckExistance=true);
CString GetTaskLibraryPath();
CString GetFlightLogPath(bool bResolveRelativePaths=true);

int strctok( char *pOutput, size_t nChar, char **ppIn, char cToken );

void SetDefaultClass(EClass eClass);
EClass GetDefaultClass();

CTime	LocalToGMT(CTime cLocalTime );
CTime	GMTToLocal(CTime cGMTTime );

void CopyHTML(const char* html);

void  WriteProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dVal );
double  GetProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault );

double GetHandicappedDistance(	ETaskType	    eTaskType,
	                            CGate			&cStartGate,
							    CLocation       *pcStartFix,
								CTurnpointArray& cTurnpointArray,
								CGate			&cFinishGate,
								double			HCP,
								int				nTurnpoints,
								int				aiTurnpoints[],
								double			adTurnpointRadii[],
								int				fCompleted,
								int				iTurnpointSought,
								CLocation		&cLandoutLocation);

double GetDistance	(	ETaskType	    m_eTaskType,
	                    CGate			&cStartGate,
					    CLocation       *pcStartFix,
						CTurnpointArray &cTurnpointArray,
						CGate			&cFinishGate,
						int				nTurnpoints,
						CLocation		acTurnpointLocations[],
						double			adTurnpointRadii[],
						int				fCompleted,
						CLocation		&cTurnpointSought,
						CLocation		&cLandoutLocation);


double GetHandicappedLegDistance(	double dRawLegDistance,
								    double dHandicap,
									double dContestantSpeedIndex,
									double dDailySpeedIndex,
									double dCourse,
									bool	fWind,
									double dWindSpeed,
									double dWindDirection );

void ImportXMLTime	(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName, CTime &cTime, int iDYear=0, int iDMonth=0, int iDDay=0);
void ImportXMLDate	(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName,  CTime &cDate);
void GetXMLDate		(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMNode, CString strChildName,  CTime cDate);
void GetXMLDateFull	(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMNode, CString strChildName,  CTime cDate);
void GetXMLTime		(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName,  CTime cTime, bool bdate=true);
void strClean( const char *pInput, CString &strOutput);

void  FormatFlightLogZipName( CString &strZipFileName, int iSSA_ID, CTime *pcTime) ;

double GetAdjustedHandicap(CGliderInfo *pcInfo, double dWeight, double dSpan, int bRootFairing, int bTurbulation, int bWinglets );
bool CheckForHandicapUpdate();
CString	GetScoresheetPathName(int iSSA_ID);

bool Rechecklateststart(CFlight* pcFlight,CWinscoreDoc *pDocument, bool bBatch,  bool bNeedAnalysis=true);

CString getNoLogKey(CString cCID, CTime cDate);

inline void TESTHR( HRESULT _hr ) 
   { if FAILED(_hr) throw(_hr); }

void	LoadViewControl( CWSView &cWSView, CWSBaro *pWSBaro, CWinscoreDoc *pDoc, CFlight *pcFlight, CTask *pcTask );


#endif
