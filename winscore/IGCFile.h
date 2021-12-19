//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// IGCFile.h: interface for the CIGCFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGCFILE_H__36BAB18B_D466_11D2_B0B9_0080C84B0161__INCLUDED_)
#define AFX_IGCFILE_H__36BAB18B_D466_11D2_B0B9_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Position.h"


#ifndef _CAIEXPLR
#include "turnpointarray.h"
#else
#include "igctask.h"
#include "waypointarray.h"
#endif

#include <fstream>
#include <afxtempl.h>
#include "xmlmgr.h"

class CIGCFile : public CObject  
{
public:
	bool IsLXNav();
	bool IsVolkslogger();
	void ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pFlightNode);
	void GetXML(CXMLMgr &cMg, MSXML2::IXMLDOMNodePtr &pParentNode);
	static bool Smash(CString strInput, CString strOutput, int nSec=10);
	bool IsCAI302File();
	bool IsCAIGPSNAVFile();
	int m_iMaxPresAltitude;
	int m_iMaxGPSAltitude;
	CString GetValidFileName();
	BOOL ValidateFileName();
	bool	ProcessCRecords(CString &strRecord, std::ifstream &cIfstream);
	bool	GetTimeRange( CTime &cTimeBegin, CTime &cTimeEnd);
	int		m_iTimeZone;
	void	AssignSpeedROC();
	double  GetAveSpeed( CArray<double,double>&,  int i, int nPoints=3);
	static CString	GetDatePrefix(int iYear, int iMonth, int iDay);
	static CString	GetLongDatePrefix(int iYear, int iMonth, int iDay);
	static bool GetARecord(CString strIGCFile, CString &strLongName );

	CPtrArray	m_caTasks;

#ifndef _CAIEXPLR
	CTurnpointArray	m_cTurnpointArray;
#else
	CIGCTask* GetIGCTask(int iTask);
	CWaypointArray	m_cWaypointArray;
#endif

	bool		ContainsFixAccuracy();
	CString		GetIGCFileNameText();
	void		SetIGCFileMissing(bool bMissing=true);
	CString		GetContestID();
	void		FreePositionData();
	CPosition*	GetPosition(int i);
	int			GetNumPoints();
	bool		ReadFlight(bool bReadWaypoints=false);
	CString		SummaryText();

	CSemaphore m_cSem_ReadFlight,m_cSem_FreePositionData;



	CIGCFile(CString strIGCFileName);
	CIGCFile();
	CIGCFile(CIGCFile *pcIGCFile);
	virtual ~CIGCFile();

	CObArray	m_PositionArray;

	CString		m_strFilePath;
	bool		m_bFileMissing;
	bool		m_bValid;
	CTime		m_mtime;
	CString		m_strErrorText;
	CString		m_strFileName;
	CString		m_strGliderType;
	CString		m_strGliderID, m_strCompetitionID;
	CString		m_strPilot;
	CString		m_strSite;
	CString		m_strUnits;
	CString		m_strManufacturer;
	CString		m_strFDRID;
	CString		m_strHardwareVersion;

	int			m_nInvalidFixes;
	int			m_nPositionPoints;
	int			m_nLinesRead;
	int			m_iDay;
	int			m_iMonth;
	int			m_iYear;

	int			m_iAccuracyStartPos;
	int			m_iAccuracyEndPos;

	int			m_iENLStartPos;
	int			m_iENLEndPos;

	int			m_iMOPStartPos;
	int			m_iMOPEndPos;

	bool		m_bCONVCAM;

private:
#ifndef _CAIEXPLR
	DECLARE_SERIAL(CIGCFile)
#endif
	bool ProcessARecord(CString strRecord);
	bool ProcessHRecord(CString strRecord);
	bool ProcessIRecord(CString strRecord);
};

#endif // !defined(AFX_IGCFILE_H__36BAB18B_D466_11D2_B0B9_0080C84B0161__INCLUDED_)

