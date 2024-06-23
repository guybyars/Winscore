//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Summary.h: interface for the CSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "task.h"
#include "wsclass.h"

#if !defined(AFX_SUMMARY_H__D3BED9D3_A24E_11D3_B124_7030F7000000__INCLUDED_)
#define AFX_SUMMARY_H__D3BED9D3_A24E_11D3_B124_7030F7000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSummary  
{
public:
	void Output(CTask *pcTask, EUnits eUnits, bool IsHandicapped, CWinscoreListCtrl& cStatus);
	CSummary();
	void Initialize();
	virtual ~CSummary();
	bool GetXML( CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMNode, CTask *pcTask, EUnits eUnits, bool bIsHandicapped );
	void SetTaskDistance(double dDist);

	//SSA Stuff

	double		m_dTaskDistance;
	double		m_dBestSpeed;
	double		m_dBestScoredSpeed;
	double		m_dSlowestSpeed;
	double		m_dSlowestScoredSpeed;
	double		m_dBestDistance;
	double      m_dBestDistanceFinisher;
	double		m_dBestHdcapSpeed;
	double		m_dBestScoredHdcapSpeed;
	double		m_dSlowestHdcapSpeed;
	double		m_dSlowestScoredHdcapSpeed;
	double		m_dBestHdcapDist;
	double      m_dBestHdcapDistFinisher;
	double		m_dDistaceOfFinisherWithBestSpeed;
	CTimeSpan   m_cTOCBest;
	CTimeSpan   m_cTOCBestSpeed;
	CTimeSpan   m_cTOCBestHdcapSpeed;
	int			m_nContestants;
	int			m_nFinishers;
	int			m_nGuestFinishers;
	int			m_nFinTocGT15underMinTime;
	int			m_nGTMinDistance;
	CString     m_strCIDBestSpeed; 
	CString     m_strCIDSlowestSpeed; 
	CString     m_strCIDBestDistance;
	CString     m_strCIDBestHCSpeed; 
	CString     m_strCIDSlowestHCSpeed; 
	CString     m_strCIDBestHCDistance;

	// FAI Stuff

	double		m_dDt;	// Task Distance. (Used in scoring RT only and defined in 6.3.1c)
	CTimeSpan	m_cTd;	// Minimum Task Time.  (For the AAT, Td is specified at Briefing; for the RT, Td = 0).
	double		m_dD1;	// Minimum Distance for 1000 points, depending on the class:
						/* Class								D1
						13.5 Metre Club							250 km
						Standard, 15 Metre, 20 Metre Multi-seat 300 km
						18 Metre, Open							350 km */
	double		m_dDm;	//Minimum Distance to validate the Day, depending on the class:
						/* Class								Dm
						13.5 Metre, Club						100 km
						Standard, 15 Metre, 20 Metre Multi-seat 120 km
						18 Metre, Open							140 km
						*/
	int			m_in1;	 // Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm
	int			m_in2;	 //	Number of finishers exceeding 2/3 of best Handicapped Speed (Vo).
	int			m_in3;	 // Number of finishers, regardless of speed
	int			m_in4;	 // Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm/2
	int			m_iN;	 // Number of competitors having had a competition launch that Day
	double		m_dHo;	 // Lowest Handicap (H) of all competitors
	double		m_dDo;	 // Highest Handicapped Distance (Dh) of the Day
	double		m_dVo;	 // Highest finisher’s Handicapped Speed (Vh) of the Day
	CTimeSpan	m_cTo;	 // Marking Time (T) of the finisher whose Vh = Vo. In case of a tie, lowest T applies.
	double		m_dPm;	 // Maximum available Score for the Day, before F and FCR are applied.
	double		m_dPdm;  // Maximum available Distance Points for the Day, before F and FCR are applied.
	double		m_dPvm;	 // Maximum available Speed Points for the Day, before F and FCR are applied.
	double		m_dF;	 // Day Factor
	double		m_dFCR;	 // Completion Ratio Factor Day If the Day is not a Championship Day (see 8.2.1) then all Scores = 0,
						 // subject to the application of penalties defined in 8.2.5.
	double		m_dSpo;  // Highest Provisional Score (SP) of the Day
	double      m_dSpm;   // Median Provisional Score (SP) of the Day, excluding competitors with SP = 0. Note: this is not the Mean Provisional Score.

	CString		m_strCIDHo;
	CString		m_strCIDDo;
	CString		m_strCIDVo;
};

#endif // !defined(AFX_SUMMARY_H__D3BED9D3_A24E_11D3_B124_7030F7000000__INCLUDED_)

