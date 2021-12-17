//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// LandingCard.h: interface for the CLandingCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDINGCARD_H__8CBF6E80_542D_11D1_ACEA_000000000000__INCLUDED_)
#define AFX_LANDINGCARD_H__8CBF6E80_542D_11D1_ACEA_000000000000__INCLUDED_

#include "Turnpoint.h"
#include "TurnpointArray.h"
#include "Contestant.h"
#include "Task.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class CLandingCard : public CObject  
{

protected:

	DECLARE_SERIAL(CLandingCard)

private:

	CString m_SeekingColumnText;
	CString m_LandedAtColumnText;
	CString m_TPAcheivedColumnText;
	CString m_DistanceColumnText;

public:
	/*
	void SetFix(int i, int iFix);
	int GetFix(int i);
	CLocation& GetLocation(int i);
	void SetLocation(int i, CLocation &cLocation);
	CString GetStartGateText(CTask *pcTask, CContestant *pcContestant);
	CGate& GetStartGate( CTask *pcTask, CContestant *pcContestant);
	CString GetDateText();
	void TurnpointArray( CTask* pcTask, int aiTurnpoints[] );
	int GetNumTurnpoints(CTask *pcTask=NULL);
	int GetTurnpointID(int i, CTask *pcTask=NULL);
	CLandingCard(CString cContestNo, CTime cTime);
	BOOL Used2Turnpoints();
	double GetDistance( CTask *pcTask,  CContestant *pcContestant, CTurnpointArray& cTurnpointArray, EUnits eUnits);

	bool	BetterDistanceUsingPFP( void* pcFlight, CTask *pcTask,  CContestant *pcContestant, CTurnpointArray& cTurnpointArray );
	
	CString SeekingText(CTurnpointArray& cTurnpointArray);
	CString LandedAtText(CTurnpointArray& cTurnpointArray);
	CString TPAcheivedText(CTask *pcTask, CTurnpointArray& cTurnpointArray);
	CString DistanceText( CTask *pcTask, CContestant* pcContestant, CTurnpointArray& cTurnpointArray, EUnits eUnits);
	CString StatusText();

	CString SeekingColumnText();
	CString LandedAtColumnText();
	CString TPAcheivedColumnText();
	CString DistanceColumnText();
	*/
	CGate		m_cStartGate;
	bool		m_bLocalStartGate;
	int			m_fCompleted;
	int			m_nTurnpoints;
	int			m_aiTurnpoints[MAXTASKTPS];
	int			m_iLandoutTurnpoint;
	int			m_iTurnpointSought;
	int			m_iOptions;
	CLocation	m_acTurnpointLocations[MAXTASKTPS];
	int			m_aiTurnpointFixes[MAXTASKTPS];
	CLocation	m_cLandoutLocation;
	CTime		m_cDate;
	CString		m_cContestNo;
	BOOL		m_bDesignatedAirfield;
	BOOL		m_bUseTPLocations;

	virtual void Serialize(CArchive& ar);

	CLandingCard();
//	CLandingCard::CLandingCard(CLandingCard* pcLandingCard );
	virtual ~CLandingCard();

};

//int CALLBACK CompareLandingCards(LPARAM lParam1, LPARAM lParam2, 
//    LPARAM iColumn);

#endif // !defined(AFX_LANDINGCARD_H__8CBF6E80_542D_11D1_ACEA_000000000000__INCLUDED_)

