//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Penality.h: interface for the CPenality class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PENALITY_H__61B64632_5F38_11D1_ACFC_000000000000__INCLUDED_)
#define AFX_PENALITY_H__61B64632_5F38_11D1_ACFC_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "xmlmgr.h"

//#define LVIF_INDENT             0x0010
//#define LVIF_NORECOMPUTE        0x0800

#define NUMCODES  10
static _TCHAR *_gszPredefinedCodes[NUMCODES] =
{
	_T("AS"),
	_T("AD"), 
	_T("FP"), 
	_T("TP"),  
	_T("SP"), 
	_T("FD"), 
	_T("TI"), 
	_T("UO"), 
	_T("UC"), 
	_T("WT"), 

};
static _TCHAR *_gszPredefinedDescriptions[NUMCODES] =
{
	_T("Airspace Violation Penalty"), 
	_T("Administrative Penalty"), 
	_T("Finish Penalty"), 
	_T("Turnpoint Penalty"),  
	_T("Start Penalty"), 
	_T("Incomplete flight documentation Penalty"), 
	_T("Time interval (LTI/ FDI) exceeded Penalty"), 
	_T("Unsafe Operation Penalty"), 
	_T("Unsportsmanlike Conduct Penalty"), 
	_T("Weight Violation Penalty"), 
};



class CPenality : public CObject  
{

DECLARE_SERIAL(CPenality)

public:
	CPenality(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pPenalty);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);
	CPenality(CString &cContestNo, CTime &cDate,  EPenality ePenality, CString &cCode, CString &cReason, double dPenality  );
	double ApplyDailyPenality(double dPoints, double dMaxDayPoints=0.0);
	double CalculateDailyPenality( double dMaxDayPoints );

	CString GetPenaltityText();
	CString GetTypeText();
	CString GetDateText();
	CPenality();
	CPenality(CPenality* pcPenality);
	virtual ~CPenality();
	virtual void Serialize(CArchive & ar);

	CString		m_cContestNo;
	CTime		m_cDate;
	EPenality	m_ePenality;
	CString		m_cPenalityCode;
	CString		m_cPenalityReason;
	double		m_dPenality;
};

int CALLBACK ComparePenalty(LPARAM lParam1, LPARAM lParam2, LPARAM iColumn);

#endif // !defined(AFX_PENALITY_H__61B64632_5F38_11D1_ACFC_000000000000__INCLUDED_)

