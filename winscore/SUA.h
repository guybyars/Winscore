//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// SUA.h: interface for the CSUASection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUA_H__AC658E2C_AF9E_4736_A5FF_D82420AFDB63__INCLUDED_)
#define AFX_SUA_H__AC658E2C_AF9E_4736_A5FF_D82420AFDB63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "location.h"

class CSUASection  
{
public:
	bool FarFrom(CLocation &cLocation, double dDist);

	enum ESUAType	{  eCTACTR,
					   eAIRWAYS,
					   eRESTRICTED,
					   ePROHIBITED,
					   eDANGER,
					   eOTHER};

	CSUASection(const TCHAR *pstr);
	CSUASection();
	virtual ~CSUASection();

	CString		m_strTitle;
	ESUAType	m_eType;
	CPtrArray	m_aSubSections;
	CString		m_strOriginalText;

};


class CSUASubSection  
{
public:

	enum ESUASubType{  ePOINT,
					   eCIRCLE,
					   eANTICLOCKWISE,
					   eCLOCKWISE,
					   eAWY};

	CSUASubSection();
	virtual ~CSUASubSection();

	int			m_iTop, m_iBase;
	ESUASubType	m_eType;
	CLocation	m_cLocation;
	CLocation	m_cCenter;
	double		m_dRadius;
	double		m_dWidth;

};

class CSUASectionArray : public CPtrArray  
{
public:
	CString m_strFileName;
	void Purge();
	CSUASection* GetAt(int i);
	CSUASectionArray();
	virtual ~CSUASectionArray();

};


#endif // !defined(AFX_SUA_H__AC658E2C_AF9E_4736_A5FF_D82420AFDB63__INCLUDED_)

/*

#############################################################
###################### USER DEFINED  FILE ###################
#############################################################
#
# This is an (almost) empty file in which you can define your own Display Data.
#
# All lines beginning # are considered comments and will be ignored.
#
# Each line consists of a Keyword and a value. These Keywords are defined
# as follows:
#
# TITLE=    Sections and Sub-Sections Always commence with a line beginning 
#           TITLE=.
#
# INCLUDE=  When a line beginning INCLUDE=NO is encountered skip all text
#           until a line beginning INCLUDE=YES is encountered.
#           This will normally be on the line BEFORE a TITLE= line.
#           YES and NO are the only permitted Values.
#
# TYPE=     This defines the Type of Airspace. The permitted values are:         
#           Definition          Abbreviation
#           CTA/CTR             C
#           AIRWAYS             A
#           RESTRICTED          R
#           PROHIBITED          P
#           DANGER              D
#           OTHER               O
#           TRAINING ZONE       Z
#           TRAFFIC INFO        I
#           
#           The latter two are not currently in use, but may be used at
#           some future date.
#
# POINT=    This is always followed by a Lat/Long position, for example
#           POINT=N524033 W0014380. It is important that the construction 
#           is followed exactly. If this is the first POINT= line AFTER a 
#           TITLE=, drawing will commence from this point, otherwise a line
#           will be drawn from the previous point to this new point. The last
#           POINT will normally be the same as the first POINT.
#
# CLOCKWISE or ANTI-CLOCKWISE
#           This is always followed by a RADIUS keyword, a CENTRE Keyword and 
#           a TO Keyword (The only valid place for RADIUS, CENTRE and TO 
#           keywords is when describing an Arc or Circle.), and describes
#           an arc to be drawn Clockwise or Anti-Clockwise from the current 
#           position, and should be constructed as follows:
# ANTI-CLOCKWISE RADIUS=8 CENTRE=N522734 W0014494 TO=N521948 W0014754
# or   CLOCKWISE RADIUS=8 CENTRE=N522734 W0014494 TO=N521948 W0014754
#           The units of Radius are Nautical Miles.
#
# CIRCLE=   Similar to Arc but with only RADIUS and CENTRE Secondary keywords.
#           e.g. CIRCLE RADIUS=10 CENTRE=N555700 W0032227
# WIDTH=    This Keyword describes the width of an Airway in Nautical Miles and 
#           is only used in conjunction with an AWY keyword. If no WIDTH=
#           statement is given then 10NM will be assumed.
# AWY=      This is like the POINT= keyword and describes a point on the
#           centre-line of an Airway whose width has been previously given in
#           a WIDTH= statement
#           e.g. WIDTH=10
#           AWY=N555223 W0042667
# TOPS=
# BASE=     Each section of airspace requires a BASE= and TOPS= statement
#           in order that Air_Check can detect infringements. The BASE can be
#           defined as a Flight Level or ALT. TOPS will normally be a Flight 
#           Level. For example:
#           WIDTH=10
#           BASE=2500ALT
#           TOPS=FL245
#           or
#           WIDTH=10
#           BASE=FL55
#           TOPS=FL245
#
# The file must finish with the word END 
#
# Further Examples of the use of WIDTH= and AWY= can be found in the file
# AIRWAYS.AIR, and other Keywords in CLASS_A.AIR and CLASS_D.AIR.
#
# You can use this file to draw any shape you wish, for instance you could
# draw your Base Airfield's ATZ or Runway. The following sample data draws the
# Main Runway at Lasham.
#
# NOTE: POINTs etc. are in the format (d)ddmmss [degrees, minutes, seconds
#      and not (d)ddmm.mm [decimal minutes] as in previous issues.
#
INCLUDE=YES
TYPE=TRAINING ZONE
#
TITLE=Lasham Runway 09/27
#
POINT=N511112 W0010238
POINT=N511114 W0010238
POINT=N511119 W0010109
POINT=N511117 W0010109
POINT=N511112 W0010238
#
#
# End of File. Have Fun!
#
END

--------------------------------------------------------------------------------


URL
www.spsys.demon.co.uk/#Airspace 
*/


