//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRecorder.h: interface for the CFDRecorder class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "igcfile.h"
#include "XMLMgr.h"


class CFDRecorder : public CObject  
{
public:
	CFDRecorder();
	CFDRecorder(CIGCFile* pcFlight);
	CFDRecorder(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pRecorder);
	virtual ~CFDRecorder();

	CString GetFDRENLMOPText() ;
	void	GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode);


	CString GetFDRID(bool bIncludeENL=false);
	int	GetENLMin() {return m_iENLMin;};
	int	GetENLMax() {return m_iENLMax;};
	int	GetMOPMin() {return m_iMOPMin;};
	int	GetMOPMax() {return m_iMOPMax;};

	CString GetManufacturer()		{return m_strManufacturer;};
	CString GetHardwareVersion()	{return m_strHardwareVersion;};
	CString GetFileName()			{return m_strFileName;};
	CString GetCID()				{return m_strCompetitionID;};
	void	SetCID(CString strID)	{m_strCompetitionID=strID;};
	CString GetENLText();
	CString GetMOPText();
	CString GetDateText();
	CTime	GetTime();

private:
	CString		m_strManufacturer;
	CString		m_strFDRID;
	CString		m_strHardwareVersion;

	CString		m_strFileName;
	CString		m_strCompetitionID;

	int			m_iYear;
	int			m_iMonth;
	int			m_iDay;

	int			m_iENLMin;
	int			m_iENLMax;
	int			m_iMOPMin;
	int			m_iMOPMax;

};

int CALLBACK CompareRecorders(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn);
