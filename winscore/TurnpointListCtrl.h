//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointListCtrl.h: interface for the CTurnpointListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TURNPOINTLISTCTRL_H__01094781_51D4_11D1_ACE8_000000000000__INCLUDED_)
#define AFX_TURNPOINTLISTCTRL_H__01094781_51D4_11D1_ACE8_000000000000__INCLUDED_

#include "gate.h"
#include "Turnpoint.h"
#include "TurnpointArray.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WinscoreListCtrl.h"
#include "Winscore.h"	// Added by ClassView

class CTurnpointListCtrl : public CWinscoreListCtrl  
{
public:
	void SetFix( int iTP, int iFix, CLocation &cFixLocation);
	bool m_bShowFinish;
	bool m_bCheckBoxes;
	bool m_bRadiusDisplay;
	bool m_bFixDisplay;

	void ShowFinish(bool bShow);

	CGate m_cStartGate;
	CGate m_cFinishGate;

	CTurnpoint* LastAcheived();

	CString				m_strUnits;
	EUnits				m_eUnits;
	CTurnpointArray*	m_pcTurnpointArray;

	void Initalize( EUnits eUnits, CString cUnitsText, CTurnpointArray *pcTurnpointArray, bool bCheckboxes=false, bool bRadiusDisplay=false, bool bFixDisplay=false );

	void AddTurnpoint(int iID, int iAttribute=TPT_CONTROL, double dRadius=1);

	void SetStartGate( CGate &g ) {m_cStartGate=g;}
	void SetFinishGate(CGate &g ) {m_cFinishGate=g;}
	void CTurnpointListCtrl::UpdateDistance();
	void ResetContent();
	int  GetTurnpointID(int i);
	float GetTurnpointRadius(int i);
	void GetTurnpointRadii(int aiTurnpointRadii [ ]);
	void GetTurnpointIDs(int aiTurnpoints [ ]);

	CLocation m_cFixLocations[MAXTASKTPS];
	int		  m_iFixIDS[MAXTASKTPS];

	double Distance(EUnits eUnits);

	int GetNumTurnpoints();

	void DeleteSelected();
	void InsertTurnpoint( CTurnpoint* pcTurnpoint, double dRadius=1);
	int GetCurSel();
	CTurnpointListCtrl();
	virtual ~CTurnpointListCtrl();




};

#endif // !defined(AFX_TURNPOINTLISTCTRL_H__01094781_51D4_11D1_ACE8_000000000000__INCLUDED_)

