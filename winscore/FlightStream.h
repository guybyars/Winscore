//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightStream.h: interface for the CFlightStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLIGHTSTREAM_H__521E6CD6_6942_11D3_B10B_0080C84B0161__INCLUDED_)
#define AFX_FLIGHTSTREAM_H__521E6CD6_6942_11D3_B10B_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "flight.h"
#include "fixlist.h"

class CFlightStream : public CObject  
{

protected:	
	DECLARE_SERIAL(CFlightStream)

private:

	CFixList	*m_pcData;

public:

	int  m_iIncrement;
	int  m_MaxPoints;	
	CTime m_cStartTime;
	CTime m_cEndhTime;
	int	 m_iPointsAvailable;
	short m_sMinAltitude;
	short m_sMaxAltitude;

	void AddFlight( CFlight* pcFlight, int iHomeElevation);
	CFlightStream();
	virtual ~CFlightStream();
	CFlightStream(CTime &cStart, CTime &cEnd, int iIncrement);
	virtual void Serialize(CArchive & ar);

};

#endif // !defined(AFX_FLIGHTSTREAM_H__521E6CD6_6942_11D3_B10B_0080C84B0161__INCLUDED_)

